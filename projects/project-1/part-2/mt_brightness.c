#include "bmp.h"
#include "threadpool.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined C_IMPLEMENTATION && \
    !defined SIMD_INTRINSICS_IMPLEMENTATION && \
    !defined SIMD_ASM_IMPLEMENTATION
#define C_IMPLEMENTATION 1
#endif

#ifndef C_IMPLEMENTATION
#include <immintrin.h>
#endif

typedef struct _filters_brightness_data
{
    uint8_t *pixels;
    size_t position;
    size_t brightness;
    size_t contrast;
    size_t channels_to_process;
    volatile ssize_t *channels_left;
    volatile bool *barrier_sense;
} filters_brightness_data_t;

static void brightness_processing_task(
                void *task_data,
                void (*result_callback)(void *result) __attribute__((unused))
            )
{
    filters_brightness_data_t *data = task_data;

    uint8_t *pixels = data->pixels;
    size_t position = data->position;
    size_t brightness = data->brightness;
    size_t contrast = data->contrast;
    size_t channels_to_process = data->channels_to_process;
    size_t end = position + channels_to_process;

#if defined SIMD_INTRINSICS_IMPLEMENTATION || defined SIMD_ASM_IMPLEMENTATION
        size_t step = 16;
#else
        size_t step = 4;
#endif

        for (; position < end; position += step) {
#if defined C_IMPLEMENTATION

            pixels[position] =
                (uint8_t) UTILS_CLAMP(pixels[position] * contrast + brightness, 0.0f, 255.0f);
            pixels[position + 1] =
                (uint8_t) UTILS_CLAMP(pixels[position + 1] * contrast + brightness, 0.0f, 255.0f);
            pixels[position + 2] =
                (uint8_t) UTILS_CLAMP(pixels[position + 2] * contrast + brightness, 0.0f, 255.0f);

#elif defined SIMD_INTRINSICS_IMPLEMENTATION

            __m512 inBrightness = _mm512_broadcastss_ps(_mm_load_ps((__mm128*) &brightness));
            __m512 inContrast = _mm512_broadcastss_ps(_mm_load_ps((__mm128*) &contrast));
            __m512i toInt = _mm512_cvtepu8_epi32(_mm_load_si128((__m128i*) &pixels[position]));
            __m512 toFloat =_mm512_cvtepi32_ps(toInt);
            toFloat = _mm512_fmadd_ps(toFloat, inContrast, inBrightness);
            toInt = _mm512_cvtps_epi32(toFloat);
            _mm512_mask_cvtusepi32_storeu_epi8(&pixels[position], 0xffff, toInt);

#elif defined SIMD_ASM_IMPLEMENTATION

            __asm__ __volatile__ (
                "vbroadcastss (%0), %%zmm2\n\t"         
                "vbroadcastss (%1), %%zmm1\n\t"          
                "vpmovzxbd (%2,%3), %%zmm0\n\t"          
                "vcvtdq2ps %%zmm0, %%zmm0\n\t"           
                "vfmadd132ps %%zmm1, %%zmm2, %%zmm0\n\t" 
                "vcvtps2dq %%zmm0, %%zmm0\n\t"           
                "vpmovusdb %%zmm0, (%2,%3)\n\t"          
            ::
                "S"(&brightness), "D"(&contrast), "b"(pixels), "c"(position)
            :
                "%zmm0", "%zmm1", "%zmm2"
            );

#endif

}

    ssize_t channels_left = __sync_sub_and_fetch(data->channels_left, (ssize_t) channels_to_process);
    if (channels_left <= 0) {
        __sync_lock_test_and_set(data->barrier_sense, true);
    }

    free(data);
    data = NULL;
}

int main(int argc, char *argv[])
{
    int result = EXIT_FAILURE;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <brightness> <contrast> <source file> <dest. file>\n", argv[0]);
        return result;
    }

    float brightness = strtof(argv[1], NULL);
    float contrast = strtof(argv[2], NULL);

    char *source_file_name = argv[3];
    char *destination_file_name = argv[4];
    FILE *source_descriptor = NULL;
    FILE *destination_descriptor = NULL;

    bmp_image image; 
    bmp_init_image_structure(&image);

    source_descriptor = fopen(source_file_name, "r");
    if (source_descriptor == NULL) {
        fprintf(stderr, "Failed to open the source image file '%s'\n", source_file_name);
        goto cleanup;
    }

    const char *error_message;
    bmp_open_image_headers(source_descriptor, &image, &error_message);
    if (error_message != NULL) {
        fprintf(stderr, "Failed to process the image '%s':\n\t%s\n", source_file_name, error_message);
        goto cleanup;
    }

    bmp_read_image_data(source_descriptor, &image, &error_message);
    if (error_message != NULL) {
        fprintf(stderr, "Failed to process the image '%s':\n\t%s\n", source_file_name, error_message);
        goto cleanup;
    }

    destination_descriptor = fopen(destination_file_name, "w");
    if (destination_descriptor == NULL) {
        fprintf(stderr, "Failed to create the output image '%s'\n", destination_file_name);
        goto cleanup;
    }

    bmp_write_image_headers(destination_descriptor, &image, &error_message);
    if (error_message != NULL) {
        fprintf(stderr, "Failed to process the image '%s':\n\t%s\n", destination_file_name, error_message);
        goto cleanup;
    }

    
    size_t pool_size = utils_get_number_of_cpu_cores();
    threadpool_t *threadpool = threadpool_create(pool_size);
    if (threadpool == NULL) {
        fputs("Failed to create a threadpool.\n", stderr);
        goto cleanup;
    }
    

    /* Main Image Processing Loop */
    {  
        static volatile ssize_t channels_left = 0;
        static volatile bool barrier_sense = false;
        
        uint8_t *pixels = image.pixels;

        size_t width = image.absolute_image_width;
        size_t height = image.absolute_image_height;

        size_t channels_count = width * height * 4;
        channels_left = channels_count;
        size_t channels_per_thread = channels_count / pool_size;

#if defined SIMD_INTRINSICS_IMPLEMENTATION || defined SIMD_ASM_IMPLEMENTATION
        channels_per_thread = ((channels_per_thread - 1) / 16 + 1) * 16;
#else
        channels_per_thread = ((channels_per_thread - 1) / 4 + 1) * 4;
#endif

        for (size_t position = 0; position < channels_count; position += channels_per_thread) {
            filters_brightness_data_t *task_data = malloc(sizeof(*task_data));
            if (task_data == NULL) {
                fputs("Out of memory.\n", stderr);
                goto cleanup;
            }

            task_data->pixels = pixels;
            task_data->position = position;
            task_data->brightness = brightness;
            task_data->contrast = contrast;

            task_data->channels_to_process =
                position + channels_per_thread > channels_count ?
                    channels_count - position :
                    channels_per_thread;
            task_data->channels_left = &channels_left;
            task_data->barrier_sense = &barrier_sense;

            threadpool_enqueue_task(threadpool, brightness_processing_task, task_data, NULL);
        }

        while (!barrier_sense) { }
    }

    bmp_write_image_data(destination_descriptor, &image, &error_message);
    if (error_message != NULL) {
        fprintf(stderr, "Failed to process the image '%s':\n\t%s\n", destination_file_name, error_message);
        goto cleanup;
    }

    result = EXIT_SUCCESS;

cleanup:
    bmp_free_image_structure(&image);

    if (source_descriptor != NULL) {
        fclose(source_descriptor);
        source_descriptor = NULL;
    }

    if (destination_descriptor != NULL) {
        fclose(destination_descriptor);
        destination_descriptor = NULL;
    }

    return result;
}