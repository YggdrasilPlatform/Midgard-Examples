/******************************************************************************/
/** \file       mandelbrot.s
 *******************************************************************************
 *
 *  \brief      Optimised assemlby calculation
 *
 *  \author     sagen1, webef4
 *
 *  \date       29.04.2021
 *
 ******************************************************************************/

// R0 : cx, R1 : cy, R2 : juliaX, R3 : juliaY, R4 : n
// S5 : cx, S6 : cy
// S0 : x, S1 : y
// S2 : xx, S3 : yy, S4 : two_xy
// S7 : Scratch
// S8 : JuliaX, S9, JuliaY


calculateFractalPoint:
	//vmov.f64 s0, r0
	//vmov.f64 s1, r1
	vmov.f64 d5, d0
	vmov.f64 d6, d1
	vmov.f64 d8, d2
	vmov.f64 d9, d3

	mov r4, #0x00
	mov r5, #64

loop:
	vmul.f64 d2, d0, d0			// xx = x * x
	vmul.f64 d3, d1, d1			// yy = y * y
	vmul.f64 d4, d0, d1 		// two_xy = x * y

	vadd.f64 d4, d4, d4 		// two_xy = two_xy * 2


	vmov.f64 d0, d2
	vsub.f64 d0, d0, d3
	//vadd.f64 d0, d0, d5 		// x = xx - yy + cx => for Mandelbrot only
	vadd.f64 d0, d0, d8 		// x = xx - yy + juliaX

	//vadd.f64 d1, d4, d6 		// y = two_xy + cy => for Mandelbrot only
	vadd.f64 d1, d4, d9 		// y = two_xy + juliaY

	add r4, r4, #0x01 			// n++

	vadd.f64 d2, d2, d3 		// xx = xx + yy
	vmov.f64 d7, #4.0 			// d5 = 4.0

	vcmpe.f64 d2, d7
	vmrs APSR_nzcv, fpscr
	bpl end						// (xx + yy) < 4

	cmp r5, r4
	bhi loop

end:

	mov r0, r4
	bx lr 						// return n
.globl calculateFractalPoint
