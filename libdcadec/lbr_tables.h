/*
 * This file is part of libdcadec.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

static const uint16_t sample_rates[16] = {
    8000, 16000,  32000,     0,
    0,    22050,  44100,     0,
    0,        0,  12000, 24000,
    48000,    0,      0,     0
};

static const uint16_t fst_amp[44] = {
       0,    1,    2,    3,
       4,    6,    8,   10,
      12,   16,   20,   24,
      28,   36,   44,   52,
      60,   76,   92,  108,
     124,  156,  188,  220,
     252,  316,  380,  444,
     508,  636,  764,  892,
    1020, 1276, 1532, 1788,
    2044, 2556, 3068, 3580,
    4092, 5116, 6140, 7164
};

static const uint8_t freq_to_sf[32] = {
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

static const int8_t ph0_shift[8] = {
    -32, +96, -96, +32, +96, -32, +32, -96
};

static const uint8_t bits_for_ch_num[10] = {
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4
};

static const uint8_t grid_1_to_scf[11] = {
    0, 1, 2, 3, 4, 6, 7, 10, 14, 19, 26
};

static const uint8_t grid_2_to_scf[3] = {
    4, 10, 18
};

static const uint8_t scf_to_grid_1[32] = {
    0, 1, 2, 3, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7,
    7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10
};

static const uint8_t scf_to_grid_2[32] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

static const uint8_t grid_2_codes_1[32] = {
    66, 99, 65, 64, 66, 132, 65, 64, 66, 99, 65, 64, 66, 165, 65, 64,
    66, 99, 65, 64, 66, 132, 65, 64, 66, 99, 65, 64, 66, 191, 65, 64
};

static const uint8_t grid_2_codes_2[16] = {
    159, 38, 71, 38, 104, 38, 71, 38, 137, 38, 71, 38, 104, 38, 71, 38
};

static const uint8_t grid_2_codes_3[32] = {
    74, 178, 108, 75, 74, 142, 112, 75, 74, 145, 108, 75, 74, 175, 112, 75,
    74, 173, 108, 75, 74, 142, 112, 75, 74, 145, 108, 75, 74, 191, 112, 75
};

static const uint8_t grid_3_codes_1[32] = {
    113, 48, 79, 48, 142, 48, 79, 48, 113, 48, 79, 48, 178, 48, 79, 48,
    113, 48, 79, 48, 142, 48, 79, 48, 113, 48, 79, 48, 191, 48, 79, 48
};

static const uint8_t grid_3_codes_2[8] = {
    45, 108, 45, 83, 45, 127, 45, 83
};

static const uint8_t grid_3_codes_3[16] = {
    75, 52, 138, 52, 75, 52, 117, 52, 75, 52, 159, 52, 75, 52, 117, 52
};

static const uint8_t grid_1_weights[12][32] = {
    {
        128,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0, 128,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0, 128,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0,   0, 128,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0,   0,   0, 128, 128,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0,   0,   0,   0,   0, 128,  85,
         43,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0,   0,   0,   0,   0,   0,  43,
         85, 128,  96,  64,  32,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,  32,  64,  96, 128, 102,  77,
         51,  26,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,  26,  51,
         77, 102, 128, 107,  85,  64,  43,  21,
          0,   0,   0,   0,   0,   0,   0,   0,
    }, {
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,  21,  43,  64,  85, 107,
        128, 110,  91,  73,  55,  37,  18,   0,
    }, {
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,  18,  37,  55,  73,  91, 110, 128,
    }
};

static const uint8_t sb_reorder[8][8] = {
    { 0, 1, 2, 3, 4, 5, 6, 7 },
    { 1, 0, 2, 3, 4, 5, 6, 7 },
    { 3, 1, 0, 2, 4, 5, 6, 7 },
    { 1, 2, 3, 0, 4, 5, 6, 7 },
    { 1, 2, 5, 3, 0, 4, 6, 7 },
    { 1, 2, 2, 5, 3, 0, 4, 6 },
    { 1, 2, 2, 6, 5, 3, 0, 4 },
    { 1, 2, 2, 6, 5, 4, 0, 3 }
};

static const int8_t lfe_delta_index_16[8] = {
    -4, -3, -2, -1, 2, 4, 6, 8
};

static const int8_t lfe_delta_index_24[32] = {
    -8, -8, -7, -7, -6, -6, -5, -5, -4, -4, -3, -3, -2, -2, -1, -1,
     1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8
};

static const uint8_t residual_code_len[64] = {
    3, 2, 3, 2, 4, 2, 3, 2, 3, 2, 3, 2, 5, 2, 3, 2,
    3, 2, 3, 2, 4, 2, 3, 2, 3, 2, 3, 2, 6, 2, 3, 2,
    3, 2, 3, 2, 4, 2, 3, 2, 3, 2, 3, 2, 5, 2, 3, 2,
    3, 2, 3, 2, 4, 2, 3, 2, 3, 2, 3, 2, 6, 2, 3, 2
};

static const uint8_t residual_code_val[64] = {
    1, 2, 4, 3, 5, 2, 0, 3, 1, 2, 4, 3, 6, 2, 0, 3,
    1, 2, 4, 3, 5, 2, 0, 3, 1, 2, 4, 3, 7, 2, 0, 3,
    1, 2, 4, 3, 5, 2, 0, 3, 1, 2, 4, 3, 6, 2, 0, 3,
    1, 2, 4, 3, 5, 2, 0, 3, 1, 2, 4, 3, 4, 2, 0, 3
};

static const uint16_t residual_pack_5_in_8[256] = {
    0x0000, 0x0100, 0x0200, 0x0040, 0x0140, 0x0240, 0x0080, 0x0180,
    0x0280, 0x0010, 0x0110, 0x0210, 0x0050, 0x0150, 0x0250, 0x0090,
    0x0190, 0x0290, 0x0020, 0x0120, 0x0220, 0x0060, 0x0160, 0x0260,
    0x00a0, 0x01a0, 0x02a0, 0x0004, 0x0104, 0x0204, 0x0044, 0x0144,
    0x0244, 0x0084, 0x0184, 0x0284, 0x0014, 0x0114, 0x0214, 0x0054,
    0x0154, 0x0254, 0x0094, 0x0194, 0x0294, 0x0024, 0x0124, 0x0224,
    0x0064, 0x0164, 0x0264, 0x00a4, 0x01a4, 0x02a4, 0x0008, 0x0108,
    0x0208, 0x0048, 0x0148, 0x0248, 0x0088, 0x0188, 0x0288, 0x0018,
    0x0118, 0x0218, 0x0058, 0x0158, 0x0258, 0x0098, 0x0198, 0x0298,
    0x0028, 0x0128, 0x0228, 0x0068, 0x0168, 0x0268, 0x00a8, 0x01a8,
    0x02a8, 0x0001, 0x0101, 0x0201, 0x0041, 0x0141, 0x0241, 0x0081,
    0x0181, 0x0281, 0x0011, 0x0111, 0x0211, 0x0051, 0x0151, 0x0251,
    0x0091, 0x0191, 0x0291, 0x0021, 0x0121, 0x0221, 0x0061, 0x0161,
    0x0261, 0x00a1, 0x01a1, 0x02a1, 0x0005, 0x0105, 0x0205, 0x0045,
    0x0145, 0x0245, 0x0085, 0x0185, 0x0285, 0x0015, 0x0115, 0x0215,
    0x0055, 0x0155, 0x0255, 0x0095, 0x0195, 0x0295, 0x0025, 0x0125,
    0x0225, 0x0065, 0x0165, 0x0265, 0x00a5, 0x01a5, 0x02a5, 0x0009,
    0x0109, 0x0209, 0x0049, 0x0149, 0x0249, 0x0089, 0x0189, 0x0289,
    0x0019, 0x0119, 0x0219, 0x0059, 0x0159, 0x0259, 0x0099, 0x0199,
    0x0299, 0x0029, 0x0129, 0x0229, 0x0069, 0x0169, 0x0269, 0x00a9,
    0x01a9, 0x02a9, 0x0002, 0x0102, 0x0202, 0x0042, 0x0142, 0x0242,
    0x0082, 0x0182, 0x0282, 0x0012, 0x0112, 0x0212, 0x0052, 0x0152,
    0x0252, 0x0092, 0x0192, 0x0292, 0x0022, 0x0122, 0x0222, 0x0062,
    0x0162, 0x0262, 0x00a2, 0x01a2, 0x02a2, 0x0006, 0x0106, 0x0206,
    0x0046, 0x0146, 0x0246, 0x0086, 0x0186, 0x0286, 0x0016, 0x0116,
    0x0216, 0x0056, 0x0156, 0x0256, 0x0096, 0x0196, 0x0296, 0x0026,
    0x0126, 0x0226, 0x0066, 0x0166, 0x0266, 0x00a6, 0x01a6, 0x02a6,
    0x000a, 0x010a, 0x020a, 0x004a, 0x014a, 0x024a, 0x008a, 0x018a,
    0x028a, 0x001a, 0x011a, 0x021a, 0x005a, 0x015a, 0x025a, 0x009a,
    0x019a, 0x029a, 0x002a, 0x012a, 0x022a, 0x006a, 0x016a, 0x026a,
    0x00aa, 0x01aa, 0x02aa, 0x0003, 0x0103, 0x0203, 0x0043, 0x0143,
    0x0243, 0x0083, 0x0183, 0x0283, 0x0013, 0x0113, 0x0213, 0x0053
};

static const uint8_t residual_pack_3_in_7[128][3] = {
    { 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 2 }, { 0, 0, 3 },
    { 0, 0, 4 }, { 0, 1, 0 }, { 0, 1, 1 }, { 0, 1, 2 },
    { 0, 1, 3 }, { 0, 1, 4 }, { 0, 2, 0 }, { 0, 2, 1 },
    { 0, 2, 2 }, { 0, 2, 3 }, { 0, 2, 4 }, { 0, 3, 0 },
    { 0, 3, 1 }, { 0, 3, 2 }, { 0, 3, 3 }, { 0, 3, 4 },
    { 0, 4, 0 }, { 0, 4, 1 }, { 0, 4, 2 }, { 0, 4, 3 },
    { 0, 4, 4 }, { 1, 0, 0 }, { 1, 0, 1 }, { 1, 0, 2 },
    { 1, 0, 3 }, { 1, 0, 4 }, { 1, 1, 0 }, { 1, 1, 1 },
    { 1, 1, 2 }, { 1, 1, 3 }, { 1, 1, 4 }, { 1, 2, 0 },
    { 1, 2, 1 }, { 1, 2, 2 }, { 1, 2, 3 }, { 1, 2, 4 },
    { 1, 3, 0 }, { 1, 3, 1 }, { 1, 3, 2 }, { 1, 3, 3 },
    { 1, 3, 4 }, { 1, 4, 0 }, { 1, 4, 1 }, { 1, 4, 2 },
    { 1, 4, 3 }, { 1, 4, 4 }, { 2, 0, 0 }, { 2, 0, 1 },
    { 2, 0, 2 }, { 2, 0, 3 }, { 2, 0, 4 }, { 2, 1, 0 },
    { 2, 1, 1 }, { 2, 1, 2 }, { 2, 1, 3 }, { 2, 1, 4 },
    { 2, 2, 0 }, { 2, 2, 1 }, { 2, 2, 2 }, { 2, 2, 3 },
    { 2, 2, 4 }, { 2, 3, 0 }, { 2, 3, 1 }, { 2, 3, 2 },
    { 2, 3, 3 }, { 2, 3, 4 }, { 2, 4, 0 }, { 2, 4, 1 },
    { 2, 4, 2 }, { 2, 4, 3 }, { 2, 4, 4 }, { 3, 0, 0 },
    { 3, 0, 1 }, { 3, 0, 2 }, { 3, 0, 3 }, { 3, 0, 4 },
    { 3, 1, 0 }, { 3, 1, 1 }, { 3, 1, 2 }, { 3, 1, 3 },
    { 3, 1, 4 }, { 3, 2, 0 }, { 3, 2, 1 }, { 3, 2, 2 },
    { 3, 2, 3 }, { 3, 2, 4 }, { 3, 3, 0 }, { 3, 3, 1 },
    { 3, 3, 2 }, { 3, 3, 3 }, { 3, 3, 4 }, { 3, 4, 0 },
    { 3, 4, 1 }, { 3, 4, 2 }, { 3, 4, 3 }, { 3, 4, 4 },
    { 4, 0, 0 }, { 4, 0, 1 }, { 4, 0, 2 }, { 4, 0, 3 },
    { 4, 0, 4 }, { 4, 1, 0 }, { 4, 1, 1 }, { 4, 1, 2 },
    { 4, 1, 3 }, { 4, 1, 4 }, { 4, 2, 0 }, { 4, 2, 1 },
    { 4, 2, 2 }, { 4, 2, 3 }, { 4, 2, 4 }, { 4, 3, 0 },
    { 4, 3, 1 }, { 4, 3, 2 }, { 4, 3, 3 }, { 4, 3, 4 },
    { 4, 4, 0 }, { 4, 4, 1 }, { 4, 4, 2 }, { 4, 4, 3 },
    { 4, 4, 4 }, { 5, 0, 0 }, { 5, 0, 1 }, { 5, 0, 2 }
};

static const float residual_level_2a[2] = {
    -0.47, 0.47
};

static const float residual_level_2b[2] = {
    -0.645, 0.645
};

static const float residual_level_3[3] = {
    -0.645, 0.0, 0.645
};

static const float residual_level_5[5] = {
    -0.875, -0.375, 0.0, 0.375, 0.875
};

static const float residual_level_8[8] = {
    -1.0, -0.625, -0.291666667, 0.0, 0.25, 0.5, 0.75, 1.0
};

static const float residual_level_16[16] = {
    -1.3125, -1.1375, -0.9625, -0.7875,
    -0.6125, -0.4375, -0.2625, -0.0875,
     0.0875,  0.2625,  0.4375,  0.6125,
     0.7875,  0.9625,  1.1375,  1.3125
};

static const float synth_env[32] = {
    0.00240763666390, 0.00960735979838, 0.02152983213390, 0.03806023374436,
    0.05903936782582, 0.08426519384873, 0.11349477331863, 0.14644660940673,
    0.18280335791818, 0.22221488349020, 0.26430163158700, 0.30865828381746,
    0.35485766137277, 0.40245483899194, 0.45099142983522, 0.5,
    0.54900857016478, 0.59754516100806, 0.64514233862723, 0.69134171618254,
    0.73569836841300, 0.77778511650980, 0.81719664208182, 0.85355339059327,
    0.88650522668137, 0.91573480615127, 0.94096063217418, 0.96193976625564,
    0.97847016786610, 0.99039264020162, 0.99759236333610, 1.0
};

static const float corr_cf[32][11] = {
    { -0.01179, 0.04281,  0.46712,  0.46345, -3.94525, 3.94525, -0.46345, -0.46712, -0.04281, 0.01179, -0.00299 },
    { -0.00929, 0.04882,  0.45252,  0.37972, -3.85446, 4.03189, -0.55069, -0.4804,  -0.03599, 0.01445, -0.00229 },
    { -0.00696, 0.05403,  0.43674,  0.29961, -3.75975, 4.11413, -0.64135, -0.49221, -0.02834, 0.01726, -0.00156 },
    { -0.00481, 0.05847,  0.41993,  0.22319, -3.66138, 4.19175, -0.73529, -0.50241, -0.01983, 0.02021, -0.0008  },
    { -0.00284, 0.06216,  0.40224,  0.15053, -3.55963, 4.26452, -0.83239, -0.51085, -0.01047, 0.02328, -0.00003 },
    { -0.00105, 0.06515,  0.38378,  0.08168, -3.45475, 4.33225, -0.93249, -0.51738, -0.00024, 0.02646,  0.00074 },
    {  0.00054, 0.06745,  0.36471,  0.01668, -3.34703, 4.39475, -1.03543, -0.52184,  0.01085, 0.02973,  0.00152 },
    {  0.00195, 0.06912,  0.34515, -0.04445, -3.23676, 4.45185, -1.14105, -0.5241,   0.0228,  0.03306,  0.00228 },
    {  0.00318, 0.07017,  0.32521, -0.10168, -3.12422, 4.50339, -1.24914, -0.524,    0.03561, 0.03643,  0.00302 },
    {  0.00422, 0.07065,  0.30503, -0.15503, -3.00969, 4.54921, -1.35952, -0.52141,  0.04925, 0.03981,  0.00373 },
    {  0.00508, 0.07061,  0.28471, -0.2045,  -2.89348, 4.58919, -1.47197, -0.51618,  0.0637,  0.04319,  0.0044  },
    {  0.00577, 0.07007,  0.26436, -0.25013, -2.77587, 4.62321, -1.58627, -0.50818,  0.07895, 0.04652,  0.00501 },
    {  0.00629, 0.06909,  0.2441,  -0.29194, -2.65716, 4.65118, -1.70219, -0.49727,  0.09494, 0.04979,  0.00556 },
    {  0.00666, 0.06769,  0.224,   -0.33,    -2.53764, 4.67302, -1.81949, -0.48335,  0.11166, 0.05295,  0.00604 },
    {  0.00687, 0.06592,  0.20416, -0.36435, -2.4176,  4.68866, -1.93791, -0.46627,  0.12904, 0.05597,  0.00642 },
    {  0.00694, 0.06383,  0.18468, -0.39506, -2.29732, 4.69806, -2.0572,  -0.44593,  0.14705, 0.05881,  0.00671 },
    {  0.00689, 0.06144,  0.16561, -0.42223, -2.1771,  4.7012,  -2.1771,  -0.42223,  0.16561, 0.06144,  0.00689 },
    {  0.00671, 0.05881,  0.14705, -0.44593, -2.0572,  4.69806, -2.29732, -0.39506,  0.18468, 0.06383,  0.00694 },
    {  0.00642, 0.05597,  0.12904, -0.46627, -1.93791, 4.68865, -2.41759, -0.36435,  0.20416, 0.06592,  0.00687 },
    {  0.00604, 0.05295,  0.11166, -0.48334, -1.81949, 4.67301, -2.53763, -0.33,     0.224,   0.06769,  0.00666 },
    {  0.00556, 0.04979,  0.09494, -0.49727, -1.70219, 4.65117, -2.65715, -0.29194,  0.24409, 0.06909,  0.00629 },
    {  0.00501, 0.04652,  0.07894, -0.50818, -1.58627, 4.62321, -2.77587, -0.25013,  0.26436, 0.07007,  0.00577 },
    {  0.0044,  0.04319,  0.0637,  -0.51618, -1.47197, 4.58919, -2.89348, -0.2045,   0.28471, 0.07061,  0.00508 },
    {  0.00373, 0.03981,  0.04925, -0.52141, -1.35952, 4.54921, -3.0097,  -0.15503,  0.30503, 0.07065,  0.00422 },
    {  0.00302, 0.03643,  0.03561, -0.524,   -1.24915, 4.50339, -3.12422, -0.10168,  0.32521, 0.07017,  0.00318 },
    {  0.00228, 0.03306,  0.0228,  -0.5241,  -1.14105, 4.45186, -3.23677, -0.04445,  0.34515, 0.06912,  0.00195 },
    {  0.00152, 0.02973,  0.01085, -0.52184, -1.03544, 4.39477, -3.34704,  0.01668,  0.36471, 0.06745,  0.00054 },
    {  0.00074, 0.02646, -0.00024, -0.51738, -0.93249, 4.33226, -3.45476,  0.08168,  0.38378, 0.06515, -0.00105 },
    { -0.00003, 0.02328, -0.01047, -0.51085, -0.83239, 4.26452, -3.55963,  0.15053,  0.40224, 0.06216, -0.00284 },
    { -0.0008,  0.02021, -0.01983, -0.50241, -0.73529, 4.19174, -3.66138,  0.22319,  0.41993, 0.05847, -0.00481 },
    { -0.00156, 0.01726, -0.02834, -0.49221, -0.64135, 4.11413, -3.75974,  0.29961,  0.43674, 0.05403, -0.00696 },
    { -0.00229, 0.01445, -0.03599, -0.4804,  -0.55069, 4.03188, -3.85445,  0.37972,  0.45251, 0.04882, -0.00929 },
};

static const float quant_amp[64] = {
   0.00048828125, 0.0014648438, 0.0023226703, 0.0032847517, 0.0046453401,
   0.0065695033, 0.0092906803, 0.013139007, 0.018581361, 0.026278013,
   0.037162721, 0.052556027, 0.074325442, 0.10511205, 0.14865088, 0.21022411,
   0.29730177, 0.42044821, 0.59460354, 0.84089643, 1.1892071, 1.6817929,
   2.3784142, 3.3635857, 4.7568283, 6.7271714, 9.5136566, 13.454343, 19.027313,
   26.908686, 38.054626, 53.817371, 76.109253, 107.63474, 152.21851, 215.26949,
   304.43701, 430.53897, 608.87402, 861.07794, 1217.748,  1722.1559, 2435.4961,
   3444.3118, 4870.9922, 6888.6235, 9741.9844, 13777.247, 19483.969, 27554.494,
   38967.938, 55108.988, 77935.875, 110217.98, 155871.75, 220435.95
};

static const float st_coeff[34] = {
   26.908686, 26.908686, 13.454342, 6.7271714, 3.3635857, 1.6817929,
   0.84089643, 0.59460348, 0.42044821, 0.2973018, 0.21022411, 0.1486509,
   0.1051121, 0.074325398, 0.052556001, 0.037162699, 0.03125, 0.026278,
   0.0185814, 0.013139, 0.0092906998, 0.0065695001, 0.0046453001, 0.0032848001,
   0.0023227001, 0.0016424, 0.0011613, 0.0005807, 0.0002903, 0.0001452,
   0.0000726, 0.0000363, 0.0000182, 0.0
};

static const float long_window[128] = {
    0.0, 0.0000074288241, 0.000052802097, 0.00017100701, 0.00039665322,
    0.0007639461, 0.0013065579, 0.0020575011, 0.0030490046, 0.0043123914,
    0.0058779628, 0.0077748829, 0.010031069, 0.012673087, 0.015726056, 0.019213546,
    0.023157494, 0.027578127, 0.032493874, 0.037921317, 0.043875102, 0.05036791,
    0.0574104, 0.065011166, 0.073176719, 0.081911445, 0.091217607, 0.10109533,
    0.1115426, 0.12255528, 0.13412713, 0.14624982, 0.15891297, 0.17210421,
    0.18580918, 0.20001164, 0.21469352, 0.22983496, 0.24541442, 0.26140878,
    0.27779338, 0.29454213, 0.31162769, 0.32902139, 0.34669363, 0.36461368,
    0.38275006, 0.40107045, 0.41954198, 0.43813133, 0.45680472, 0.47552827,
    0.49426794, 0.5129897, 0.53165978, 0.55024469, 0.56871128, 0.58702713,
    0.60516042, 0.62308013, 0.64075619, 0.65815955, 0.67526239, 0.69203806,
    0.70846134, 0.72450846, 0.74015719, 0.75538683, 0.77017856, 0.78451514,
    0.79838127, 0.81176347, 0.82465017, 0.8370316, 0.84890026, 0.86025023,
    0.87107784, 0.88138127, 0.89116055, 0.90041775, 0.90915662, 0.91738296,
    0.92510408, 0.932329, 0.93906844, 0.94533449, 0.95114064, 0.95650178,
    0.96143377, 0.96595365, 0.9700793, 0.97382939, 0.97722322, 0.98028058,
    0.98302156, 0.98546654, 0.98763585, 0.98954982, 0.9912284, 0.99269134,
    0.99395788, 0.99504662, 0.99597543, 0.99676138, 0.99742073, 0.99796873,
    0.99841964, 0.99878657, 0.99908173, 0.99931598, 0.99949932, 0.99964064,
    0.99974757, 0.99982697, 0.99988467, 0.99992549, 0.99995363, 0.99997234,
    0.99998432, 0.99999166, 0.99999589, 0.99999815, 0.99999928, 0.99999976,
    0.99999994, 1.0, 1.0, 1.0
};

static const float lfe_step_size_16[101] = {
    2.1362956633198035E-004, 2.4414807580797754E-004, 2.7466658528397473E-004,
    2.7466658528397473E-004, 3.0518509475997192E-004, 3.3570360423596911E-004,
    3.9674062318796350E-004, 4.2725913266396069E-004, 4.5777764213995788E-004,
    5.1881466109195227E-004, 5.7985168004394665E-004, 6.1037018951994385E-004,
    6.7140720847193823E-004, 7.6296273689992981E-004, 8.2399975585192419E-004,
    9.1555528427991577E-004, 1.0071108127079073E-003, 1.0986663411358989E-003,
    1.2207403790398877E-003, 1.3428144169438765E-003, 1.4648884548478652E-003,
    1.6174810022278512E-003, 1.7700735496078372E-003, 1.9531846064638203E-003,
    2.1362956633198035E-003, 2.3499252296517838E-003, 2.5940733054597613E-003,
    2.8687398907437361E-003, 3.1434064760277108E-003, 3.4485915707876827E-003,
    3.7842951750236518E-003, 4.1810357982116153E-003, 4.6082949308755760E-003,
    5.0660725730155339E-003, 5.5543687246314890E-003, 6.1037018951994385E-003,
    6.7445905941953795E-003, 7.4159978026673177E-003, 8.1484420300912512E-003,
    8.9419232764671782E-003, 9.8574785607470940E-003, 1.0834070863979004E-002,
    1.1932737205114903E-002, 1.3122959074678793E-002, 1.4435254982146673E-002,
    1.5869624927518540E-002, 1.7456587420270394E-002, 1.9196142460402233E-002,
    2.1118808557390057E-002, 2.3224585711233862E-002, 2.5543992431409649E-002,
    2.8107547227393413E-002, 3.0915250099185155E-002, 3.4028138065736867E-002,
    3.7415692617572556E-002, 4.1169469283120215E-002, 4.5258949552903834E-002,
    4.9806207464827418E-002, 5.4780724509414958E-002, 6.0274056215094456E-002,
    6.6286202581865905E-002, 7.2908719138157288E-002, 8.0202642902920618E-002,
    8.8229010895107887E-002, 9.7048860133671075E-002, 1.0675374614703818E-001,
    1.1743522446363720E-001, 1.2918485061189611E-001, 1.4209418012024294E-001,
    1.5628528702658162E-001, 1.7191076387829218E-001, 1.8912320322275461E-001,
    2.0804467909787286E-001, 2.2882778405102694E-001, 2.5171666615802485E-001,
    2.7689443647572254E-001, 3.0457472457045198E-001, 3.3503219702749720E-001,
    3.6854152043214211E-001, 4.0537736136967073E-001, 4.4593646046327096E-001,
    4.9052400280770286E-001, 5.3956724753563035E-001, 5.9352397228919340E-001,
    6.5288247322000792E-001, 7.1816156498916595E-001, 7.9000213629566329E-001,
    8.6898403881954400E-001, 9.5590075380718409E-001, 1.0514847254860074E+000,
    1.1566209906308176E+000, 1.2722861415448470E+000, 1.3995178075502792E+000,
    1.5394756920072024E+000, 1.6934110538041323E+000, 1.8627582628864405E+000,
    2.0490432447279274E+000, 2.2539445173497725E+000, 2.4793237098300120E+000,
    2.7272865993224893E+000, 3.0000000000000000E+000
};

static const float lfe_step_size_24[144] = {
    3.5762791128491298E-006, 3.9339070241340428E-006, 4.4107442391805934E-006,
    4.7683721504655064E-006, 5.2452093655120570E-006, 5.8412558843202453E-006,
    6.4373024031284336E-006, 7.0333489219366219E-006, 7.7486047445064479E-006,
    8.4638605670762738E-006, 9.4175349971693751E-006, 1.0252000123500839E-005,
    1.1324883857355578E-005, 1.2516976894971954E-005, 1.3709069932588331E-005,
    1.5139581577727983E-005, 1.6570093222867636E-005, 1.8239023475530564E-005,
    2.0146372335716766E-005, 2.2053721195902969E-005, 2.4318697967374082E-005,
    2.6702884042606836E-005, 2.9444698029124504E-005, 3.2305721319403807E-005,
    3.5643581824729662E-005, 3.9100651633817152E-005, 4.3034558657951193E-005,
    4.7326093593370149E-005, 5.2094465743835655E-005, 5.7339675109347712E-005,
    6.3061721689906320E-005, 6.9379814789273121E-005, 7.6293954407448102E-005,
    8.3923349848192912E-005, 9.2268001111507552E-005, 1.0156632680491529E-004,
    1.1169911762465449E-004, 1.2290479217824841E-004, 1.3518335046569711E-004,
    1.4865400179076216E-004, 1.6355516476096688E-004, 1.7988683937631122E-004,
    1.9788744424431852E-004, 2.1767618866875036E-004, 2.3949149125713007E-004,
    2.6345256131321922E-004, 2.8979781744454115E-004, 3.1876567825861912E-004,
    3.5059456236297636E-004, 3.8564209766889782E-004, 4.2426591208766842E-004,
    4.6670442422681142E-004, 5.1331526199761173E-004, 5.6469447191887759E-004,
    6.2108047259813216E-004, 6.8318851985794547E-004, 7.5149545091336386E-004,
    8.2671652158695713E-004, 9.0932856909377204E-004, 1.0002852678639017E-003,
    1.1003018737199156E-003, 1.2103320610919071E-003, 1.3314487137137310E-003,
    1.4646055060154803E-003, 1.6109945310347714E-003, 1.7721655097205054E-003,
    1.9493105351102991E-003, 2.1442177467605765E-003, 2.3586752842277626E-003,
    2.5945904963720436E-003, 2.8539899413573674E-003, 3.1393770145627278E-003,
    3.4533743206708813E-003, 3.7987236736683454E-003, 4.1785245154529228E-003,
    4.5963531251374630E-003, 5.0560242004423382E-003, 5.5617100669992049E-003,
    6.1178214690472445E-003, 6.7296036159519689E-003, 7.4025401356864135E-003,
    8.1428299120461841E-003, 8.9571486660419298E-003, 9.8527681652031147E-003,
    1.0838033060793050E-002, 1.1921884050593860E-002, 1.3114096297513997E-002,
    1.4425517848195773E-002, 1.5868069633015350E-002, 1.7454864675386508E-002,
    1.9200327301064409E-002, 2.1120431556753107E-002, 2.3232462791498040E-002,
    2.5555613703204836E-002, 2.8111222757246822E-002, 3.0922297349250002E-002,
    3.4014586688826884E-002, 3.7415985753057691E-002, 4.1157608170224208E-002,
    4.5273428591898514E-002, 4.9800759530157987E-002, 5.4780847404104160E-002,
    6.0258872539862694E-002, 6.6284783635709721E-002, 7.2913297762071824E-002,
    8.0204615617348624E-002, 8.8225017574431602E-002, 9.7047578936526643E-002,
    1.0675228914645780E-001, 1.1742748229831246E-001, 1.2917031397465634E-001,
    1.4208735729305236E-001, 1.5629603341770570E-001, 1.7192568444319778E-001,
    1.8911816944100493E-001, 2.0803001022696618E-001, 2.2883310661710579E-001,
    2.5171640535788598E-001, 2.7688804589367461E-001, 3.0457679087839018E-001,
    3.3503452957088109E-001, 3.6853794676517804E-001, 4.0539174144169587E-001,
    4.4593089174400469E-001, 4.9052399283933557E-001, 5.3957635636047796E-001,
    5.9353406352210802E-001, 6.5288742219059737E-001, 7.1817609288407480E-001,
    7.8999373793527339E-001, 8.6899314749159184E-001, 9.5589243839889027E-001,
    1.0514817299225008E+000, 1.1566298194682383E+000, 1.2722928848615747E+000,
    1.3995221137430804E+000, 1.5394743131964581E+000, 1.6934218041207556E+000,
    1.8627639845328312E+000, 2.0490403233814627E+000, 2.2539444272451910E+000,
    2.4793389414952922E+000, 2.7272728356448215E+000, 2.9999998807906962E+000
};

static const float lfe_iir[5][4] = {
    { -0.98618466, 1.9861259, 1.0, -1.9840510 },
    { -0.98883152, 1.9887193, 1.0, -1.9979848 },
    { -0.99252087, 1.9923381, 1.0, -1.9990897 },
    { -0.99591690, 1.9956781, 1.0, -1.9993745 },
    { -0.99872285, 1.9984550, 1.0, -1.9994639 }
};
