/*********************************************************************/
/*          Copyright ARM Ltd. 2010 - 2019.                          */
/* Distributed under the Boost Software License, Version 1.0.        */
/*    (See accompanying file LICENSE.txt or copy at                  */
/*          http://www.boost.org/LICENSE_1_0.txt)                    */
/*********************************************************************/

#ifndef __ARM_NEON
#error Please specify advsimd flags.
#endif

#if !defined(SLEEF_GENHEADER)
#include <arm_neon.h>
#include <stdint.h>

#include "misc.h"
#endif // #if !defined(SLEEF_GENHEADER)

#define ENABLE_DP
//@#define ENABLE_DP
#define LOG2VECTLENDP 1
//@#define LOG2VECTLENDP 1
#define VECTLENDP (1 << LOG2VECTLENDP)
//@#define VECTLENDP (1 << LOG2VECTLENDP)

#define ENABLE_SP
//@#define ENABLE_SP
#define LOG2VECTLENSP 2
//@#define LOG2VECTLENSP 2
#define VECTLENSP (1 << LOG2VECTLENSP)
//@#define VECTLENSP (1 << LOG2VECTLENSP)

#if CONFIG == 1
#define ENABLE_FMA_DP
//@#define ENABLE_FMA_DP
#define ENABLE_FMA_SP
//@#define ENABLE_FMA_SP
#endif

#define FULL_FP_ROUNDING
//@#define FULL_FP_ROUNDING
#define ACCURATE_SQRT
//@#define ACCURATE_SQRT

#define ISANAME "AArch64 AdvSIMD"

// Mask definition
typedef uint32x4_t vmask;
typedef uint32x4_t vopmask;

// Single precision definitions
typedef float32x4_t vfloat;
typedef int32x4_t vint2;

// Double precision definitions
typedef float64x2_t vdouble;
typedef int32x2_t vint;

typedef struct {
  vmask x, y;
} vmask2;

#define DFTPRIORITY 10

static INLINE int vavailability_i(int name) { return 3; }
static INLINE void vprefetch_v_p(const void *ptr) { }

static INLINE VECTOR_CC int vtestallones_i_vo32(vopmask g) {
  uint32x2_t x0 = vand_u32(vget_low_u32(g), vget_high_u32(g));
  uint32x2_t x1 = vpmin_u32(x0, x0);
  return vget_lane_u32(x1, 0);
}

static INLINE VECTOR_CC int vtestallones_i_vo64(vopmask g) {
  uint32x2_t x0 = vand_u32(vget_low_u32(g), vget_high_u32(g));
  uint32x2_t x1 = vpmin_u32(x0, x0);
  return vget_lane_u32(x1, 0);
}

// Vector load / store
static INLINE VECTOR_CC vdouble vload_vd_p(const double *ptr) { return vld1q_f64(ptr); }
static INLINE VECTOR_CC vdouble vloadu_vd_p(const double *ptr) { return vld1q_f64(ptr); }
static INLINE VECTOR_CC void vstore_v_p_vd(double *ptr, vdouble v) { vst1q_f64(ptr, v); }
static INLINE VECTOR_CC void vstoreu_v_p_vd(double *ptr, vdouble v) { vst1q_f64(ptr, v); }
static INLINE VECTOR_CC vfloat vload_vf_p(const float *ptr) { return vld1q_f32(ptr); }
static INLINE VECTOR_CC vfloat vloadu_vf_p(const float *ptr) { return vld1q_f32(ptr); }
static INLINE VECTOR_CC void vstore_v_p_vf(float *ptr, vfloat v) { vst1q_f32(ptr, v); }
static INLINE VECTOR_CC void vstoreu_v_p_vf(float *ptr, vfloat v) { vst1q_f32(ptr, v); }
static INLINE VECTOR_CC vint2 vloadu_vi2_p(int32_t *p) { return vld1q_s32(p); }
static INLINE VECTOR_CC void vstoreu_v_p_vi2(int32_t *p, vint2 v) { vst1q_s32(p, v); }
static INLINE VECTOR_CC vint vloadu_vi_p(int32_t *p) { return vld1_s32(p); }
static INLINE VECTOR_CC void vstoreu_v_p_vi(int32_t *p, vint v) { vst1_s32(p, v); }

static INLINE VECTOR_CC vdouble vgather_vd_p_vi(const double *ptr, vint vi) {
  return ((vdouble) { ptr[vget_lane_s32(vi, 0)], ptr[vget_lane_s32(vi, 1)]} );
}

static INLINE VECTOR_CC vfloat vgather_vf_p_vi2(const float *ptr, vint2 vi2) {
  return ((vfloat) {
      ptr[vgetq_lane_s32(vi2, 0)],
      ptr[vgetq_lane_s32(vi2, 1)],
      ptr[vgetq_lane_s32(vi2, 2)],
      ptr[vgetq_lane_s32(vi2, 3)]
    });
}

// Basic logical operations for mask
static INLINE VECTOR_CC vmask vand_vm_vm_vm(vmask x, vmask y) { return vandq_u32(x, y); }
static INLINE VECTOR_CC vmask vandnot_vm_vm_vm(vmask x, vmask y) {
  return vbicq_u32(y, x);
}
static INLINE VECTOR_CC vmask vor_vm_vm_vm(vmask x, vmask y) { return vorrq_u32(x, y); }
static INLINE VECTOR_CC vmask vxor_vm_vm_vm(vmask x, vmask y) { return veorq_u32(x, y); }

// Mask <--> single precision reinterpret
static INLINE VECTOR_CC vmask vreinterpret_vm_vf(vfloat vf) {
  return vreinterpretq_u32_f32(vf);
}
static INLINE VECTOR_CC vfloat vreinterpret_vf_vm(vmask vm) {
  return vreinterpretq_f32_u32(vm);
}
static INLINE VECTOR_CC vint2 vcast_vi2_vm(vmask vm) { return vreinterpretq_s32_u32(vm); }
static INLINE VECTOR_CC vmask vcast_vm_vi2(vint2 vi) { return vreinterpretq_u32_s32(vi); }

// Mask <--> double precision reinterpret
static INLINE VECTOR_CC vmask vreinterpret_vm_vd(vdouble vd) {
  return vreinterpretq_u32_f64(vd);
}
static INLINE VECTOR_CC vdouble vreinterpret_vd_vm(vmask vm) {
  return vreinterpretq_f64_u32(vm);
}
static INLINE VECTOR_CC vfloat vreinterpret_vf_vi2(vint2 vm) {
  return vreinterpretq_f32_s32(vm);
}
static INLINE VECTOR_CC vint2 vreinterpret_vi2_vf(vfloat vf) {
  return vreinterpretq_s32_f32(vf);
}
static INLINE VECTOR_CC vint2 vreinterpret_vi2_vd(vdouble vd) {
  return vreinterpretq_s32_f64(vd);
}

/****************************************/
/* Single precision FP operations */
/****************************************/
// Broadcast
static INLINE VECTOR_CC vfloat vcast_vf_f(float f) { return vdupq_n_f32(f); }

// Add, Sub, Mul, Reciprocal 1/x, Division, Square root
static INLINE VECTOR_CC vfloat vadd_vf_vf_vf(vfloat x, vfloat y) {
  return vaddq_f32(x, y);
}
static INLINE VECTOR_CC vfloat vsub_vf_vf_vf(vfloat x, vfloat y) {
  return vsubq_f32(x, y);
}
static INLINE VECTOR_CC vfloat vmul_vf_vf_vf(vfloat x, vfloat y) {
  return vmulq_f32(x, y);
}
static INLINE VECTOR_CC vfloat vrec_vf_vf(vfloat d) {
  return vdivq_f32(vcast_vf_f(1.0f), d);
}
static INLINE VECTOR_CC vfloat vdiv_vf_vf_vf(vfloat n, vfloat d) {
  return vdivq_f32(n, d);
}
static INLINE VECTOR_CC vfloat vsqrt_vf_vf(vfloat d) { return vsqrtq_f32(d); }

// |x|, -x
static INLINE VECTOR_CC vfloat vabs_vf_vf(vfloat f) { return vabsq_f32(f); }
static INLINE VECTOR_CC vfloat vneg_vf_vf(vfloat f) { return vnegq_f32(f); }

#if CONFIG == 1
// Multiply accumulate: z = z + x * y
static INLINE VECTOR_CC vfloat vmla_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) {
  return vfmaq_f32(z, x, y);
}
// Multiply subtract: z = z - x * y
static INLINE VECTOR_CC vfloat vmlanp_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) {
  return vfmsq_f32(z, x, y);
}
// Multiply subtract: z = x * y - z
static INLINE VECTOR_CC vfloat vmlapn_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) {
  return vneg_vf_vf(vfmsq_f32(z, x, y));
}
#else
static INLINE VECTOR_CC vfloat vmla_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { return vadd_vf_vf_vf(vmul_vf_vf_vf(x, y), z); }
static INLINE VECTOR_CC vfloat vmlanp_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { return vsub_vf_vf_vf(z, vmul_vf_vf_vf(x, y)); }
static INLINE VECTOR_CC vfloat vmlapn_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { return vsub_vf_vf_vf(vmul_vf_vf_vf(x, y), z); }
#endif

// max, min
static INLINE VECTOR_CC vfloat vmax_vf_vf_vf(vfloat x, vfloat y) {
  return vmaxq_f32(x, y);
}
static INLINE VECTOR_CC vfloat vmin_vf_vf_vf(vfloat x, vfloat y) {
  return vminq_f32(x, y);
}

// Comparisons
static INLINE VECTOR_CC vmask veq_vm_vf_vf(vfloat x, vfloat y) { return vceqq_f32(x, y); }
static INLINE VECTOR_CC vmask vneq_vm_vf_vf(vfloat x, vfloat y) {
  return vmvnq_u32(vceqq_f32(x, y));
}
static INLINE VECTOR_CC vmask vlt_vm_vf_vf(vfloat x, vfloat y) { return vcltq_f32(x, y); }
static INLINE VECTOR_CC vmask vle_vm_vf_vf(vfloat x, vfloat y) { return vcleq_f32(x, y); }
static INLINE VECTOR_CC vmask vgt_vm_vf_vf(vfloat x, vfloat y) { return vcgtq_f32(x, y); }
static INLINE VECTOR_CC vmask vge_vm_vf_vf(vfloat x, vfloat y) { return vcgeq_f32(x, y); }

// Conditional select
static INLINE VECTOR_CC vfloat vsel_vf_vm_vf_vf(vmask mask, vfloat x, vfloat y) {
  return vbslq_f32(mask, x, y);
}

// int <--> float conversions
static INLINE VECTOR_CC vint2 vtruncate_vi2_vf(vfloat vf) { return vcvtq_s32_f32(vf); }
static INLINE VECTOR_CC vfloat vcast_vf_vi2(vint2 vi) { return vcvtq_f32_s32(vi); }
static INLINE VECTOR_CC vint2 vcast_vi2_i(int i) { return vdupq_n_s32(i); }
static INLINE VECTOR_CC vint2 vrint_vi2_vf(vfloat d) {
  return vcvtq_s32_f32(vrndnq_f32(d));
}

/***************************************/
/* Single precision integer operations */
/***************************************/

// Add, Sub, Neg (-x)
static INLINE VECTOR_CC vint2 vadd_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vaddq_s32(x, y);
}
static INLINE VECTOR_CC vint2 vsub_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vsubq_s32(x, y);
}
static INLINE VECTOR_CC vint2 vneg_vi2_vi2(vint2 e) { return vnegq_s32(e); }

// Logical operations
static INLINE VECTOR_CC vint2 vand_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vandq_s32(x, y);
}
static INLINE VECTOR_CC vint2 vandnot_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vbicq_s32(y, x);
}
static INLINE VECTOR_CC vint2 vor_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vorrq_s32(x, y);
}
static INLINE VECTOR_CC vint2 vxor_vi2_vi2_vi2(vint2 x, vint2 y) {
  return veorq_s32(x, y);
}

// Shifts
#define vsll_vi2_vi2_i(x, c) vshlq_n_s32(x, c)
//@#define vsll_vi2_vi2_i(x, c) vshlq_n_s32(x, c)
#define vsrl_vi2_vi2_i(x, c)                                                   \
  vreinterpretq_s32_u32(vshrq_n_u32(vreinterpretq_u32_s32(x), c))
//@#define vsrl_vi2_vi2_i(x, c) vreinterpretq_s32_u32(vshrq_n_u32(vreinterpretq_u32_s32(x), c))

#define vsra_vi2_vi2_i(x, c) vshrq_n_s32(x, c)
//@#define vsra_vi2_vi2_i(x, c) vshrq_n_s32(x, c)
#define vsra_vi_vi_i(x, c) vshr_n_s32(x, c)
//@#define vsra_vi_vi_i(x, c) vshr_n_s32(x, c)
#define vsll_vi_vi_i(x, c) vshl_n_s32(x, c)
//@#define vsll_vi_vi_i(x, c) vshl_n_s32(x, c)
#define vsrl_vi_vi_i(x, c)                                                     \
  vreinterpret_s32_u32(vshr_n_u32(vreinterpret_u32_s32(x), c))
//@#define vsrl_vi_vi_i(x, c) vreinterpret_s32_u32(vshr_n_u32(vreinterpret_u32_s32(x), c))

// Comparison returning masks
static INLINE VECTOR_CC vmask veq_vm_vi2_vi2(vint2 x, vint2 y) { return vceqq_s32(x, y); }
static INLINE VECTOR_CC vmask vgt_vm_vi2_vi2(vint2 x, vint2 y) { return vcgeq_s32(x, y); }
// Comparison returning integers
static INLINE VECTOR_CC vint2 vgt_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vreinterpretq_s32_u32(vcgeq_s32(x, y));
}
static INLINE VECTOR_CC vint2 veq_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vreinterpretq_s32_u32(vceqq_s32(x, y));
}

// Conditional select
static INLINE VECTOR_CC vint2 vsel_vi2_vm_vi2_vi2(vmask m, vint2 x, vint2 y) {
  return vbslq_s32(m, x, y);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/****************************************/
/* Double precision FP operations */
/****************************************/
// Broadcast
static INLINE VECTOR_CC vdouble vcast_vd_d(double f) { return vdupq_n_f64(f); }

// Add, Sub, Mul, Reciprocal 1/x, Division, Square root
static INLINE VECTOR_CC vdouble vadd_vd_vd_vd(vdouble x, vdouble y) {
  return vaddq_f64(x, y);
}
static INLINE VECTOR_CC vdouble vsub_vd_vd_vd(vdouble x, vdouble y) {
  return vsubq_f64(x, y);
}
static INLINE VECTOR_CC vdouble vmul_vd_vd_vd(vdouble x, vdouble y) {
  return vmulq_f64(x, y);
}
static INLINE VECTOR_CC vdouble vrec_vd_vd(vdouble d) {
  return vdivq_f64(vcast_vd_d(1.0f), d);
}
static INLINE VECTOR_CC vdouble vdiv_vd_vd_vd(vdouble n, vdouble d) {
  return vdivq_f64(n, d);
}
static INLINE VECTOR_CC vdouble vsqrt_vd_vd(vdouble d) { return vsqrtq_f64(d); }

// |x|, -x
static INLINE VECTOR_CC vdouble vabs_vd_vd(vdouble f) { return vabsq_f64(f); }
static INLINE VECTOR_CC vdouble vneg_vd_vd(vdouble f) { return vnegq_f64(f); }

// max, min
static INLINE VECTOR_CC vdouble vmax_vd_vd_vd(vdouble x, vdouble y) {
  return vmaxq_f64(x, y);
}
static INLINE VECTOR_CC vdouble vmin_vd_vd_vd(vdouble x, vdouble y) {
  return vminq_f64(x, y);
}

#if CONFIG == 1
// Multiply accumulate: z = z + x * y
static INLINE VECTOR_CC vdouble vmla_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vfmaq_f64(z, x, y);
}

static INLINE VECTOR_CC vdouble vmlanp_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vfmsq_f64(z, x, y);
}

//[z = x * y - z]
static INLINE VECTOR_CC vdouble vmlapn_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vneg_vd_vd(vfmsq_f64(z, x, y));
}
#else
static INLINE VECTOR_CC vdouble vmla_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { return vadd_vd_vd_vd(vmul_vd_vd_vd(x, y), z); }
static INLINE VECTOR_CC vdouble vmlapn_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { return vsub_vd_vd_vd(vmul_vd_vd_vd(x, y), z); }
#endif

static INLINE VECTOR_CC vdouble vfma_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { // z + x * y
  return vfmaq_f64(z, x, y);
}

static INLINE VECTOR_CC vdouble vfmanp_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { // z - x * y
  return vfmsq_f64(z, x, y);
}

static INLINE VECTOR_CC vdouble vfmapn_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { // x * y - z
  return vneg_vd_vd(vfmanp_vd_vd_vd_vd(x, y, z));
}

static INLINE VECTOR_CC vfloat vfma_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { // z + x * y
  return vfmaq_f32(z, x, y);
}

static INLINE VECTOR_CC vfloat vfmanp_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { // z - x * y
  return vfmsq_f32(z, x, y);
}

static INLINE VECTOR_CC vfloat vfmapn_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { // x * y - z
  return vneg_vf_vf(vfmanp_vf_vf_vf_vf(x, y, z));
}

/* Comparisons */
static INLINE VECTOR_CC vopmask veq_vo_vd_vd(vdouble x, vdouble y) {
  return vreinterpretq_u32_u64(vceqq_f64(x, y));
}
static INLINE VECTOR_CC vopmask vneq_vo_vd_vd(vdouble x, vdouble y) {
  return vmvnq_u32(vreinterpretq_u32_u64(vceqq_f64(x, y)));
}
static INLINE VECTOR_CC vopmask vlt_vo_vd_vd(vdouble x, vdouble y) {
  return vreinterpretq_u32_u64(vcltq_f64(x, y));
}
static INLINE VECTOR_CC vopmask vgt_vo_vd_vd(vdouble x, vdouble y) {
  return vreinterpretq_u32_u64(vcgtq_f64(x, y));
}
static INLINE VECTOR_CC vopmask vle_vo_vd_vd(vdouble x, vdouble y) {
  return vreinterpretq_u32_u64(vcleq_f64(x, y));
}
static INLINE VECTOR_CC vopmask vge_vo_vd_vd(vdouble x, vdouble y) {
  return vreinterpretq_u32_u64(vcgeq_f64(x, y));
}

// Conditional select
static INLINE VECTOR_CC vdouble vsel_vd_vo_vd_vd(vopmask mask, vdouble x, vdouble y) {
  return vbslq_f64(vreinterpretq_u64_u32(mask), x, y);
}

#if 1
static INLINE CONST VECTOR_CC vdouble vsel_vd_vo_d_d(vopmask o, double v1, double v0) {
  return vsel_vd_vo_vd_vd(o, vcast_vd_d(v1), vcast_vd_d(v0));
}

static INLINE VECTOR_CC vdouble vsel_vd_vo_vo_d_d_d(vopmask o0, vopmask o1, double d0, double d1, double d2) {
  return vsel_vd_vo_vd_vd(o0, vcast_vd_d(d0), vsel_vd_vo_d_d(o1, d1, d2));
}

static INLINE VECTOR_CC vdouble vsel_vd_vo_vo_vo_d_d_d_d(vopmask o0, vopmask o1, vopmask o2, double d0, double d1, double d2, double d3) {
  return vsel_vd_vo_vd_vd(o0, vcast_vd_d(d0), vsel_vd_vo_vd_vd(o1, vcast_vd_d(d1), vsel_vd_vo_d_d(o2, d2, d3)));
}
#else
// This implementation is slower on the current CPU models (as of May 2017.)
// I(Naoki Shibata) expect that on future CPU models with hardware similar to Super Shuffle Engine, this implementation will be faster.
static INLINE CONST VECTOR_CC vdouble vsel_vd_vo_d_d(vopmask o, double d0, double d1) {
  uint8x16_t idx = vbslq_u8(vreinterpretq_u8_u32(o), (uint8x16_t) { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7 },
			    (uint8x16_t) { 8, 9, 10, 11, 12, 13, 14, 15, 8, 9, 10, 11, 12, 13, 14, 15 });
  
  uint8x16_t tab = (uint8x16_t) (float64x2_t) { d0, d1 };
  return (vdouble) vqtbl1q_u8(tab, idx);
}

static INLINE VECTOR_CC vdouble vsel_vd_vo_vo_vo_d_d_d_d(vopmask o0, vopmask o1, vopmask o2, double d0, double d1, double d2, double d3) {
  uint8x16_t idx = vbslq_u8(vreinterpretq_u8_u32(o0), (uint8x16_t) { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7 },
			    vbslq_u8(vreinterpretq_u8_u32(o1), (uint8x16_t) { 8, 9, 10, 11, 12, 13, 14, 15, 8, 9, 10, 11, 12, 13, 14, 15 },
				     vbslq_u8(vreinterpretq_u8_u32(o2), (uint8x16_t) { 16, 17, 18, 19, 20, 21, 22, 23, 16, 17, 18, 19, 20, 21, 22, 23 },
					      (uint8x16_t) { 24, 25, 26, 27, 28, 29, 30, 31, 24, 25, 26, 27, 28, 29, 30, 31 })));
  
  uint8x16x2_t tab = { { (uint8x16_t) (float64x2_t) { d0, d1 }, (uint8x16_t) (float64x2_t) { d2, d3 } } }; 
  return (vdouble) vqtbl2q_u8(tab, idx);
}

static INLINE VECTOR_CC vdouble vsel_vd_vo_vo_d_d_d(vopmask o0, vopmask o1, double d0, double d1, double d2) {
  return vsel_vd_vo_vo_vo_d_d_d_d(o0, o1, o1, d0, d1, d2, d2);
}
#endif

static INLINE VECTOR_CC vdouble vrint_vd_vd(vdouble d) { return vrndnq_f64(d); }
static INLINE VECTOR_CC vfloat vrint_vf_vf(vfloat d) { return vrndnq_f32(d); }

/****************************************/
/* int <--> float conversions           */
/****************************************/
static INLINE VECTOR_CC vint vtruncate_vi_vd(vdouble vf) {
  return vmovn_s64(vcvtq_s64_f64(vf));
}
static INLINE VECTOR_CC vdouble vcast_vd_vi(vint vi) {
  return vcvtq_f64_s64(vmovl_s32(vi));
}
static INLINE VECTOR_CC vint vcast_vi_i(int i) { return vdup_n_s32(i); }
static INLINE VECTOR_CC vint vrint_vi_vd(vdouble d) {
  return vqmovn_s64(vcvtq_s64_f64(vrndnq_f64(d)));
}

/***************************************/
/* Integer operations */
/***************************************/

// Add, Sub, Neg (-x)
static INLINE VECTOR_CC vint vadd_vi_vi_vi(vint x, vint y) { return vadd_s32(x, y); }
static INLINE VECTOR_CC vint vsub_vi_vi_vi(vint x, vint y) { return vsub_s32(x, y); }
static INLINE VECTOR_CC vint vneg_vi_vi(vint e) { return vneg_s32(e); }

// Logical operations
static INLINE VECTOR_CC vint vand_vi_vi_vi(vint x, vint y) { return vand_s32(x, y); }
static INLINE VECTOR_CC vint vandnot_vi_vi_vi(vint x, vint y) { return vbic_s32(y, x); }
static INLINE VECTOR_CC vint vor_vi_vi_vi(vint x, vint y) { return vorr_s32(x, y); }
static INLINE VECTOR_CC vint vxor_vi_vi_vi(vint x, vint y) { return veor_s32(x, y); }

// Comparison returning masks
static INLINE VECTOR_CC vopmask veq_vo_vi_vi(vint x, vint y) {
  return vcombine_u32(vceq_s32(x, y), vdup_n_u32(0));
}

// Conditional select
static INLINE VECTOR_CC vint vsel_vi_vm_vi_vi(vmask m, vint x, vint y) {
  return vbsl_s32(vget_low_u32(m), x, y);
}

/***************************************/
/* Predicates                          */
/***************************************/
static INLINE VECTOR_CC vopmask visinf_vo_vd(vdouble d) {
  const float64x2_t inf = vdupq_n_f64(SLEEF_INFINITY);
  const float64x2_t neg_inf = vdupq_n_f64(-SLEEF_INFINITY);
  uint64x2_t cmp = vorrq_u64(vceqq_f64(d, inf), vceqq_f64(d, neg_inf));
  return vreinterpretq_u32_u64(cmp);
}

static INLINE VECTOR_CC vopmask visnan_vo_vd(vdouble d) {
  return vmvnq_u32(vreinterpretq_u32_u64(vceqq_f64(d, d)));
}

static INLINE VECTOR_CC vopmask vispinf_vo_vd(vdouble d) {
  return vreinterpretq_u32_u64(vceqq_f64(d, vdupq_n_f64(SLEEF_INFINITY)));
}

static INLINE VECTOR_CC vopmask visminf_vo_vd(vdouble d) {
  return vreinterpretq_u32_u64(vceqq_f64(d, vdupq_n_f64(-SLEEF_INFINITY)));
}

static INLINE VECTOR_CC vfloat vsel_vf_vo_vf_vf(vopmask mask, vfloat x, vfloat y) {
  return vbslq_f32(mask, x, y);
}

static INLINE CONST VECTOR_CC vfloat vsel_vf_vo_f_f(vopmask o, float v1, float v0) {
  return vsel_vf_vo_vf_vf(o, vcast_vf_f(v1), vcast_vf_f(v0));
}

static INLINE VECTOR_CC vfloat vsel_vf_vo_vo_f_f_f(vopmask o0, vopmask o1, float d0, float d1, float d2) {
  return vsel_vf_vo_vf_vf(o0, vcast_vf_f(d0), vsel_vf_vo_f_f(o1, d1, d2));
}

static INLINE VECTOR_CC vfloat vsel_vf_vo_vo_vo_f_f_f_f(vopmask o0, vopmask o1, vopmask o2, float d0, float d1, float d2, float d3) {
  return vsel_vf_vo_vf_vf(o0, vcast_vf_f(d0), vsel_vf_vo_vf_vf(o1, vcast_vf_f(d1), vsel_vf_vo_f_f(o2, d2, d3)));
}

static INLINE VECTOR_CC vopmask veq_vo_vf_vf(vfloat x, vfloat y) {
  return vceqq_f32(x, y);
}
static INLINE VECTOR_CC vopmask vneq_vo_vf_vf(vfloat x, vfloat y) {
  return vmvnq_u32(vceqq_f32(x, y));
}
static INLINE VECTOR_CC vopmask vlt_vo_vf_vf(vfloat x, vfloat y) {
  return vcltq_f32(x, y);
}
static INLINE VECTOR_CC vopmask vle_vo_vf_vf(vfloat x, vfloat y) {
  return vcleq_f32(x, y);
}
static INLINE VECTOR_CC vopmask vgt_vo_vf_vf(vfloat x, vfloat y) {
  return vcgtq_f32(x, y);
}
static INLINE VECTOR_CC vopmask vge_vo_vf_vf(vfloat x, vfloat y) {
  return vcgeq_f32(x, y);
}

static INLINE VECTOR_CC vopmask veq_vo_vi2_vi2(vint2 x, vint2 y) {
  return vceqq_s32(x, y);
}
static INLINE VECTOR_CC vopmask vgt_vo_vi2_vi2(vint2 x, vint2 y) {
  return vcgtq_s32(x, y);
}
static INLINE VECTOR_CC vopmask vgt_vo_vi_vi(vint x, vint y) {
  return vcombine_u32(vcgt_s32(x, y), vdup_n_u32(0));
}
static INLINE VECTOR_CC vopmask visinf_vo_vf(vfloat d) {
  return veq_vo_vf_vf(vabs_vf_vf(d), vcast_vf_f(SLEEF_INFINITYf));
}
static INLINE VECTOR_CC vopmask vispinf_vo_vf(vfloat d) {
  return veq_vo_vf_vf(d, vcast_vf_f(SLEEF_INFINITYf));
}
static INLINE VECTOR_CC vopmask visminf_vo_vf(vfloat d) {
  return veq_vo_vf_vf(d, vcast_vf_f(-SLEEF_INFINITYf));
}
static INLINE VECTOR_CC vopmask visnan_vo_vf(vfloat d) { return vneq_vo_vf_vf(d, d); }

static INLINE VECTOR_CC vopmask vcast_vo32_vo64(vopmask m) {
  return vuzpq_u32(m, m).val[0];
}
static INLINE VECTOR_CC vopmask vcast_vo64_vo32(vopmask m) {
  return vzipq_u32(m, m).val[0];
}

static INLINE VECTOR_CC vopmask vand_vo_vo_vo(vopmask x, vopmask y) {
  return vandq_u32(x, y);
}
static INLINE VECTOR_CC vopmask vandnot_vo_vo_vo(vopmask x, vopmask y) {
  return vbicq_u32(y, x);
}
static INLINE VECTOR_CC vopmask vor_vo_vo_vo(vopmask x, vopmask y) {
  return vorrq_u32(x, y);
}
static INLINE VECTOR_CC vopmask vxor_vo_vo_vo(vopmask x, vopmask y) {
  return veorq_u32(x, y);
}

static INLINE VECTOR_CC vint2 vsel_vi2_vo_vi2_vi2(vopmask m, vint2 x, vint2 y) {
  return vbslq_s32(m, x, y);
}
static INLINE VECTOR_CC vint2 vand_vi2_vo_vi2(vopmask x, vint2 y) {
  return vandq_s32(vreinterpretq_s32_u32(x), y);
}
static INLINE VECTOR_CC vint2 vandnot_vi2_vo_vi2(vopmask x, vint2 y) {
  return vbicq_s32(y, vreinterpretq_s32_u32(x));
}
static INLINE VECTOR_CC vint vandnot_vi_vo_vi(vopmask x, vint y) {
  return vbic_s32(y, vget_low_s32(vreinterpretq_s32_u32(x)));
}
static INLINE VECTOR_CC vmask vand_vm_vo32_vm(vopmask x, vmask y) {
  return vandq_u32(x, y);
}
static INLINE VECTOR_CC vmask vand_vm_vo64_vm(vopmask x, vmask y) {
  return vandq_u32(x, y);
}
static INLINE VECTOR_CC vmask vandnot_vm_vo32_vm(vopmask x, vmask y) {
  return vbicq_u32(y, x);
}
static INLINE VECTOR_CC vmask vandnot_vm_vo64_vm(vopmask x, vmask y) {
  return vbicq_u32(y, x);
}
static INLINE VECTOR_CC vmask vor_vm_vo32_vm(vopmask x, vmask y) {
  return vorrq_u32(x, y);
}
static INLINE VECTOR_CC vmask vor_vm_vo64_vm(vopmask x, vmask y) {
  return vorrq_u32(x, y);
}
static INLINE VECTOR_CC vmask vxor_vm_vo32_vm(vopmask x, vmask y) {
  return veorq_u32(x, y);
}

static INLINE VECTOR_CC vfloat vtruncate_vf_vf(vfloat vd) { return vrndq_f32(vd); }

static INLINE VECTOR_CC vmask vcast_vm_i_i(int i0, int i1) {
  return vreinterpretq_u32_u64(vdupq_n_u64((0xffffffff & (uint64_t)i1) | (((uint64_t)i0) << 32)));
}

static INLINE VECTOR_CC vopmask veq64_vo_vm_vm(vmask x, vmask y) {
  return vreinterpretq_u32_u64(vceqq_s64(vreinterpretq_s64_u32(x), vreinterpretq_s64_u32(y)));
}

static INLINE VECTOR_CC vmask vadd64_vm_vm_vm(vmask x, vmask y) {
  return vreinterpretq_u32_s64(vaddq_s64(vreinterpretq_s64_u32(x), vreinterpretq_s64_u32(y)));
}

static INLINE VECTOR_CC vint vsel_vi_vo_vi_vi(vopmask m, vint x, vint y) {
  return vbsl_s32(vget_low_u32(m), x, y);
}

// Logical operations
static INLINE VECTOR_CC vint vand_vi_vo_vi(vopmask x, vint y) {
  return vand_s32(vreinterpret_s32_u32(vget_low_u32(x)), y);
}

static INLINE VECTOR_CC vint2 vcastu_vi2_vi(vint vi) {
  return vreinterpretq_s32_u32(vrev64q_u32(vreinterpretq_u32_u64(vmovl_u32(vreinterpret_u32_s32(vi)))));
}
static INLINE VECTOR_CC vint vcastu_vi_vi2(vint2 vi2) {
  return vreinterpret_s32_u32(vmovn_u64(vreinterpretq_u64_u32(vrev64q_u32(vreinterpretq_u32_s32(vi2)))));
}
static INLINE VECTOR_CC vdouble vreinterpret_vd_vi2(vint2 vi) {
  return vreinterpretq_f64_s32(vi);
}
static INLINE VECTOR_CC vdouble vtruncate_vd_vd(vdouble vd) { return vrndq_f64(vd); }

//

#define PNMASK ((vdouble) { +0.0, -0.0 })
#define NPMASK ((vdouble) { -0.0, +0.0 })
#define PNMASKf ((vfloat) { +0.0f, -0.0f, +0.0f, -0.0f })
#define NPMASKf ((vfloat) { -0.0f, +0.0f, -0.0f, +0.0f })

static INLINE VECTOR_CC vdouble vposneg_vd_vd(vdouble d) { return vreinterpret_vd_vm(vxor_vm_vm_vm(vreinterpret_vm_vd(d), vreinterpret_vm_vd(PNMASK))); }
static INLINE VECTOR_CC vdouble vnegpos_vd_vd(vdouble d) { return vreinterpret_vd_vm(vxor_vm_vm_vm(vreinterpret_vm_vd(d), vreinterpret_vm_vd(NPMASK))); }
static INLINE VECTOR_CC vfloat vposneg_vf_vf(vfloat d) { return (vfloat)vxor_vm_vm_vm((vmask)d, (vmask)PNMASKf); }
static INLINE VECTOR_CC vfloat vnegpos_vf_vf(vfloat d) { return (vfloat)vxor_vm_vm_vm((vmask)d, (vmask)NPMASKf); }

static INLINE VECTOR_CC vdouble vsubadd_vd_vd_vd(vdouble x, vdouble y) { return vadd_vd_vd_vd(x, vnegpos_vd_vd(y)); }
static INLINE VECTOR_CC vfloat vsubadd_vf_vf_vf(vfloat d0, vfloat d1) { return vadd_vf_vf_vf(d0, vnegpos_vf_vf(d1)); }
static INLINE VECTOR_CC vdouble vmlsubadd_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { return vsubadd_vd_vd_vd(vmul_vd_vd_vd(x, y), z); }
static INLINE VECTOR_CC vfloat vmlsubadd_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { return vsubadd_vf_vf_vf(vmul_vf_vf_vf(x, y), z); }

static INLINE VECTOR_CC vdouble vrev21_vd_vd(vdouble d0) { return (float64x2_t)vcombine_u64(vget_high_u64((uint64x2_t)d0), vget_low_u64((uint64x2_t)d0)); }
static INLINE VECTOR_CC vdouble vreva2_vd_vd(vdouble vd) { return vd; }

static INLINE VECTOR_CC void vstream_v_p_vd(double *ptr, vdouble v) { vstore_v_p_vd(ptr, v); }
static INLINE VECTOR_CC void vscatter2_v_p_i_i_vd(double *ptr, int offset, int step, vdouble v) { vstore_v_p_vd((double *)(&ptr[2*offset]), v); }
static INLINE VECTOR_CC void vsscatter2_v_p_i_i_vd(double *ptr, int offset, int step, vdouble v) { vstore_v_p_vd((double *)(&ptr[2*offset]), v); }

static INLINE VECTOR_CC vfloat vrev21_vf_vf(vfloat d0) { return vrev64q_f32(d0); }
static INLINE VECTOR_CC vfloat vreva2_vf_vf(vfloat d0) { return vcombine_f32(vget_high_f32(d0), vget_low_f32(d0)); }
static INLINE VECTOR_CC vint2 vrev21_vi2_vi2(vint2 i) { return vreinterpret_vi2_vf(vrev21_vf_vf(vreinterpret_vf_vi2(i))); }

static INLINE VECTOR_CC void vstream_v_p_vf(float *ptr, vfloat v) { vstore_v_p_vf(ptr, v); }

static INLINE VECTOR_CC void vscatter2_v_p_i_i_vf(float *ptr, int offset, int step, vfloat v) {
  vst1_f32((float *)(ptr+(offset + step * 0)*2), vget_low_f32(v));
  vst1_f32((float *)(ptr+(offset + step * 1)*2), vget_high_f32(v));
}

static INLINE VECTOR_CC void vsscatter2_v_p_i_i_vf(float *ptr, int offset, int step, vfloat v) {
  vst1_f32((float *)(ptr+(offset + step * 0)*2), vget_low_f32(v));
  vst1_f32((float *)(ptr+(offset + step * 1)*2), vget_high_f32(v));
}

//

static INLINE vmask2 vinterleave_vm2_vm2(vmask2 v) {
  return (vmask2) {
    vreinterpretq_u32_u64(vtrn1q_u64(vreinterpretq_u64_u32(v.x), vreinterpretq_u64_u32(v.y))),
    vreinterpretq_u32_u64(vtrn2q_u64(vreinterpretq_u64_u32(v.x), vreinterpretq_u64_u32(v.y))) };
}

static INLINE vmask2 vuninterleave_vm2_vm2(vmask2 v) {
  return (vmask2) {
    vreinterpretq_u32_u64(vtrn1q_u64(vreinterpretq_u64_u32(v.x), vreinterpretq_u64_u32(v.y))),
    vreinterpretq_u32_u64(vtrn2q_u64(vreinterpretq_u64_u32(v.x), vreinterpretq_u64_u32(v.y))) };
}

static INLINE vint vuninterleave_vi_vi(vint v) { return v; }
static INLINE vdouble vinterleave_vd_vd(vdouble vd) { return vd; }
static INLINE vdouble vuninterleave_vd_vd(vdouble vd) { return vd; }
static INLINE vmask vinterleave_vm_vm(vmask vm) { return vm; }
static INLINE vmask vuninterleave_vm_vm(vmask vm) { return vm; }

static vmask2 vloadu_vm2_p(void *p) {
  vmask2 vm2 = {
    vld1q_u32((uint32_t *)p),
    vld1q_u32((uint32_t *)((uint8_t *)p + sizeof(vmask)))
  };
  return vm2;
}

#if !defined(SLEEF_GENHEADER)
typedef Sleef_quad2 vargquad;

static INLINE vmask2 vcast_vm2_aq(vargquad aq) {
  union {
    vargquad aq;
    vmask2 vm2;
  } c;
  c.aq = aq;
  return vinterleave_vm2_vm2(c.vm2);
}

static INLINE vargquad vcast_aq_vm2(vmask2 vm2) {
  union {
    vargquad aq;
    vmask2 vm2;
  } c;
  c.vm2 = vuninterleave_vm2_vm2(vm2);
  return c.aq;
}
#endif

static INLINE int vtestallzeros_i_vo64(vopmask g) {
  uint32x2_t x0 = vorr_u32(vget_low_u32(g), vget_high_u32(g));
  uint32x2_t x1 = vpmax_u32(x0, x0);
  return ~vget_lane_u32(x1, 0);
}

static INLINE vmask vsel_vm_vo64_vm_vm(vopmask m, vmask x, vmask y) { return vbslq_u32(m, x, y); }

static INLINE vmask vsub64_vm_vm_vm(vmask x, vmask y) {
  return vreinterpretq_u32_s64(vsubq_s64(vreinterpretq_s64_u32(x), vreinterpretq_s64_u32(y)));
}

static INLINE vmask vneg64_vm_vm(vmask x) {
  return vreinterpretq_u32_s64(vnegq_s64(vreinterpretq_s64_u32(x)));
}

static INLINE vopmask vgt64_vo_vm_vm(vmask x, vmask y) {
  return vreinterpretq_u32_u64(vcgtq_s64(vreinterpretq_s64_u32(x), vreinterpretq_s64_u32(y)));
}

#define vsll64_vm_vm_i(x, c) vreinterpretq_u32_u64(vshlq_n_u64(vreinterpretq_u64_u32(x), c))
//@#define vsll64_vm_vm_i(x, c) vreinterpretq_u32_u64(vshlq_n_u64(vreinterpretq_u64_u32(x), c))
#define vsrl64_vm_vm_i(x, c) vreinterpretq_u32_u64(vshrq_n_u64(vreinterpretq_u64_u32(x), c))
//@#define vsrl64_vm_vm_i(x, c) vreinterpretq_u32_u64(vshrq_n_u64(vreinterpretq_u64_u32(x), c))

static INLINE vmask vcast_vm_vi(vint vi) {
  vmask m = vreinterpretq_u32_u64(vmovl_u32(vreinterpret_u32_s32(vi)));
  return vor_vm_vm_vm(vcast_vm_vi2(vcastu_vi2_vi(vreinterpret_s32_u32(vget_low_u32(vgt_vo_vi_vi(vcast_vi_i(0), vi))))), m);
}
static INLINE vint vcast_vi_vm(vmask vm) { return vreinterpret_s32_u32(vmovn_u64(vreinterpretq_u64_u32(vm))); }
