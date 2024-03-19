#include <stdint.h>
#include <math.h>
#include "FTP_GFX.h"

#define D_WIDTH 5
#define D_BORDER 1
#define D_FULL (D_WIDTH + D_BORDER)


// void render_circle(uint8_t x, uint8_t y,uint8_t w, uint8_t h, uint8_t mode, uint16_t border_color, uint8_t infill_toggle,uint16_t infill_color){
//     uint8_t fillCount = 0;
//     uint8_t i = 0;
//     uint8_t j = 0;
//     for (j = y; j < h; j++) {
//         for (i = x; i < w; i++) {
//             uint8_t filled = isFilled(x, y,w ,h, mode);
//             // r_Pixel(x,y,border_color) += add(x, y, w, h, filled);

//             if (filled) {
//                 fillCount++;
//             }
//         }
//     }
// }

uint8_t isFilled(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t mode) {
    const bounds = getBounds();

    x = -.5 * (w - 2 * (x + .5));
    y = -.5 * (h - 2 * (y + .5));

    switch (mode) {
        // Thick
        case 0: {
            return fatfilled(x, y, (w / 2), w / h);
        }
        // Thin
        case 1: {
            return thinfilled(x, y, (w / 2), w / y);
        }
        // Normal
        default: {
            return filled(x, y, (w / 2), w / y);
        }
    }
}
// Was string return?
uint8_t add(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t filled, uint8_t dFull) {
		uint8_t xp = (((x + 1) * dFull) /*+ (this._svg_width / 2)*/ - (dFull / 2)) + .5;
		uint8_t yp = (((y + 1) * dFull) /*+ (this._svg_height / 2)*/ - (dFull / 2)) + .5;

		uint16_t color = 0;

		uint8_t midx = (w / 2) - .5;
		uint8_t midy = (h / 2) - .5;

		char extra[50] = "";
		if (filled) {
			if (x == midx || y == midy) {
				color = 0x808080;
			} else {
				color = 0xFF0000;
			}

		} else if (x == midx || y == midy) {
			if (xor(!!(x & 1), !!(y & 1))) {
				color = 0xEEEEEE;
			} else {
				color = 0xF8F8F8;
			}
		}

		return ;
	}

uint16_t distance(uint8_t x, uint8_t y, uint8_t ratio) {
	return sqrt((pow(y * ratio, 2)) + pow(x, 2));
}

uint8_t xor(uint8_t left, uint8_t right) {
	return (left) ? (!right) : (right);
}

uint8_t filled(uint8_t x, uint8_t y, uint8_t radius, uint8_t ratio) {
	return distance(x, y, ratio) <= radius;
}

uint8_t fatfilled(uint8_t x, uint8_t y, uint8_t radius, uint8_t ratio) {
	return filled(x, y, radius, ratio) && !(
		filled(x + 1, y, radius, ratio) &&
		filled(x - 1, y, radius, ratio) &&
		filled(x, y + 1, radius, ratio) &&
		filled(x, y - 1, radius, ratio) &&
		filled(x + 1, y + 1, radius, ratio) &&
		filled(x + 1, y - 1, radius, ratio) &&
		filled(x - 1, y - 1, radius, ratio) &&
		filled(x - 1, y + 1, radius, ratio)
	);
}

uint8_t thinfilled(uint8_t x, uint8_t y, uint8_t radius, uint8_t ratio) {
	return filled(x, y, radius, ratio) && !(
		filled(x + 1, y, radius, ratio) &&
		filled(x - 1, y, radius, ratio) &&
		filled(x, y + 1, radius, ratio) &&
		filled(x, y - 1, radius, ratio)
	);
}