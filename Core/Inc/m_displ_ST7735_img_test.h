/*
 * m_displ_ST7735_img_test.h
 *
 *  Created on: May 8, 2026
 *      Author: Користувач
 */

#ifndef INC_M_DISPL_ST7735_IMG_TEST_H_
#define INC_M_DISPL_ST7735_IMG_TEST_H_

#include <stdio.h>
#include <stdlib.h>

void Displ_ImgTest();
void Displ_ImgPartTest(
    uint16_t x,
    uint16_t y,
    uint16_t w,
    uint16_t h
);
void Displ_ImgDifftTest(
    uint16_t o_x,
    uint16_t o_y,
    uint16_t w,
    uint16_t h,
    uint16_t n_x,
    uint16_t n_y
);

#endif /* INC_M_DISPL_ST7735_IMG_TEST_H_ */
