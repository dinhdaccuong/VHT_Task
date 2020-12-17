#ifndef __UTILS_H__
#define __UTILS_H__


/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include <stdint.h>

/*******************************************************************************
**                                DEFINES
*******************************************************************************/
// clang-format off

// clang-format on
/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/

/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/
void incrase_brightness(uint8_t* image, int size, int brightness);

uint8_t saturate_cast(uint8_t num);

int convert_chars_to_int(const uint8_t* chars, int pos = 0);

void convert_int_to_chars(uint8_t* chars, int num, int pos = 0);

void print_chars(const uint8_t* chars, int n_char);

void copy_array(uint8_t* arr1, uint8_t* arr2, uint32_t size);

int receive_all_data(int soket, uint8_t* buff, uint32_t size);

int send_all_data(int socket, uint8_t* buff, uint32_t size);

void show_image_data(uint8_t *image, uint32_t size);

#endif /* __UTILS_H__ */

/******************************** End of file *********************************/
