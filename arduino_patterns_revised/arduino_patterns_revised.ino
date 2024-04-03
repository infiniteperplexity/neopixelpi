// As of 8/10/2022, I archived all other versions of this code; the multiple versions were getting too confusing.

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
  #include <math.h>
#endif

#define PIN 6
#define NLEDS 3680

// if I did everything right, this should correctly map every single grid position to either zero or one pixels.
int pixel_map[72][72] = {
  { // 0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18
      19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 1
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
      72,  71,  70,  69,  68,  67,  66,  65,  64,  63,  62,  61,  60,  59,  58,  57,  56,  55
      54,  53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40,  39,  38,  37
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 2
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  73
      74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91
      92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109
     110,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 3
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 150, 149
     148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131
     130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113
     112, 111,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 4
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 151, 152, 153
     154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171
     172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189
     190, 191, 192,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 5
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 236, 235, 234, 233
     232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215
     214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197
     196, 195, 194, 193,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 6
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 237, 238, 239, 240, 241
     242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259
     260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277
     278, 279, 280, 281, 282,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 7
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 330, 329, 328, 327, 326, 325
     324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307
     306, 305, 304, 303, 302, 301, 300, 299, 298, 297, 296, 295, 294, 293, 292, 291, 290, 289
     288, 287, 286, 285, 284, 283,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 8
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 331, 332, 333, 334, 335, 336, 337
     338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355
     356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373
     374, 375, 376, 377, 378, 379, 380,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 9
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 432, 431, 430, 429, 428, 427, 426, 425
     424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408, 407
     406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389
     388, 387, 386, 385, 384, 383, 382, 381,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 10
       0,   0,   0,   0,   0,   0,   0,   0,   0, 433, 434, 435, 436, 437, 438, 439, 440, 441
     442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459
     460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477
     478, 479, 480, 481, 482, 483, 484, 485, 486,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 11
       0,   0,   0,   0,   0,   0,   0,   0, 542, 541, 540, 539, 538, 537, 536, 535, 534, 533
     532, 531, 530, 529, 528, 527, 526, 525, 524, 523, 522, 521, 520, 519, 518, 517, 516, 515
     514, 513, 512, 511, 510, 509, 508, 507, 506, 505, 504, 503, 502, 501, 500, 499, 498, 497
     496, 495, 494, 493, 492, 491, 490, 489, 488, 487,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 12
       0,   0,   0,   0,   0,   0,   0, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553
     554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571
     572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589
     590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600,   0,   0,   0,   0,   0,   0,   0
  },
  { // 13
       0,   0,   0,   0,   0,   0, 660, 659, 658, 657, 656, 655, 654, 653, 652, 651, 650, 649
     648, 647, 646, 645, 644, 643, 642, 641, 640, 639, 638, 637, 636, 635, 634, 633, 632, 631
     630, 629, 628, 627, 626, 625, 624, 623, 622, 621, 620, 619, 618, 617, 616, 615, 614, 613
     612, 611, 610, 609, 608, 607, 606, 605, 604, 603, 602, 601,   0,   0,   0,   0,   0,   0
  },
  { // 14
       0,   0,   0,   0,   0, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673
     674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691
     692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707, 708, 709
     710, 711, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722,   0,   0,   0,   0,   0
  },
  { // 15
       0,   0,   0,   0, 786, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775, 774, 773
     772, 771, 770, 769, 768, 767, 766, 765, 764, 763, 762, 761, 760, 759, 758, 757, 756, 755
     754, 753, 752, 751, 750, 749, 748, 747, 746, 745, 744, 743, 742, 741, 740, 739, 738, 737
     736, 735, 734, 733, 732, 731, 730, 729, 728, 727, 726, 725, 724, 723,   0,   0,   0,   0
  },
  { // 16
       0,   0,   0, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799, 800, 801
     802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819
     820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837
     838, 839, 840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 852,   0,   0,   0
  },
  { // 17
       0,   0, 920, 919, 918, 917, 916, 915, 914, 913, 912, 911, 910, 909, 908, 907, 906, 905
     904, 903, 902, 901, 900, 899, 898, 897, 896, 895, 894, 893, 892, 891, 890, 889, 888, 887
     886, 885, 884, 883, 882, 881, 880, 879, 878, 877, 876, 875, 874, 873, 872, 871, 870, 869
     868, 867, 866, 865, 864, 863, 862, 861, 860, 859, 858, 857, 856, 855, 854, 853,   0,   0
  },
  { // 18
     921, 922, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938
     939, 940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954,   0,   0
       0,   0,3680,3679,3678,3677,3676,3675,3674,3673,3672,3671,3670,3669,3668,3667,3666,3665
    3664,3663,3662,3661,3660,3659,3658,3657,3656,3655,3654,3653,3652,3651,3650,3649,3648,3647
  },
  { // 19
     987, 986, 985, 984, 983, 982, 981, 980, 979, 978, 977, 976, 975, 974, 973, 972, 971, 970
     969, 968, 967, 966, 965, 964, 963, 962, 961, 960, 959, 958, 957, 956, 955,   0,   0,   0
       0,   0,   0,3614,3615,3616,3617,3618,3619,3620,3621,3622,3623,3624,3625,3626,3627,3628
    3629,3630,3631,3632,3633,3634,3635,3636,3637,3638,3639,3640,3641,3642,3643,3644,3645,3646
  },
  { // 20
     988, 989, 990, 991, 992, 993, 994, 995, 996, 997, 998, 999,1000,1001,1002,1003,1004,1005
    1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,   0,   0,   0,   0
       0,   0,   0,   0,3613,3612,3611,3610,3609,3608,3607,3606,3605,3604,3603,3602,3601,3600
    3599,3598,3597,3596,3595,3594,3593,3592,3591,3590,3589,3588,3587,3586,3585,3584,3583,3582
  },
  { // 21
    1050,1049,1048,1047,1046,1045,1044,1043,1042,1041,1040,1039,1038,1037,1036,1035,1034,1033
    1032,1031,1030,1029,1028,1027,1026,1025,1024,1023,1022,1021,1020,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,3551,3552,3553,3554,3555,3556,3557,3558,3559,3560,3561,3562,3563
    3564,3565,3566,3567,3568,3569,3570,3571,3572,3573,3574,3575,3576,3577,3578,3579,3580,3581
  },
  { // 22
    1051,1052,1053,1054,1055,1056,1057,1058,1059,1060,1061,1062,1063,1064,1065,1066,1067,1068
    1069,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,1080,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,3550,3549,3548,3547,3546,3545,3544,3543,3542,3541,3540,3539
    3538,3537,3536,3535,3534,3533,3532,3531,3530,3529,3528,3527,3526,3525,3524,3523,3522,3521
  },
  { // 23
    1109,1108,1107,1106,1105,1104,1103,1102,1101,1100,1099,1098,1097,1096,1095,1094,1093,1092
    1091,1090,1089,1088,1087,1086,1085,1084,1083,1082,1081,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,3492,3493,3494,3495,3496,3497,3498,3499,3500,3501,3502
    3503,3504,3505,3506,3507,3508,3509,3510,3511,3512,3513,3514,3515,3516,3517,3518,3519,3520
  },
  { // 24
    1110,1111,1112,1113,1114,1115,1116,1117,1118,1119,1120,1121,1122,1123,1124,1125,1126,1127
    1128,1129,1130,1131,1132,1133,1134,1135,1136,1137,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,3491,3490,3489,3488,3487,3486,3485,3484,3483,3482
    3481,3480,3479,3478,3477,3476,3475,3474,3473,3472,3471,3470,3469,3468,3467,3466,3465,3464
  },
  { // 25
    1164,1163,1162,1161,1160,1159,1158,1157,1156,1155,1154,1153,1152,1151,1150,1149,1148,1147
    1146,1145,1144,1143,1142,1141,1140,1139,1138,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,3437,3438,3439,3440,3441,3442,3443,3444,3445
    3446,3447,3448,3449,3450,3451,3452,3453,3454,3455,3456,3457,3458,3459,3460,3461,3462,3463
  },
  { // 26
    1165,1166,1167,1168,1169,1170,1171,1172,1173,1174,1175,1176,1177,1178,1179,1180,1181,1182
    1183,1184,1185,1186,1187,1188,1189,1190,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3436,3435,3434,3433,3432,3431,3430,3429
    3428,3427,3426,3425,3424,3423,3422,3421,3420,3419,3418,3417,3416,3415,3414,3413,3412,3411
  },
  { // 27
    1215,1214,1213,1212,1211,1210,1209,1208,1207,1206,1205,1204,1203,1202,1201,1200,1199,1198
    1197,1196,1195,1194,1193,1192,1191,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3386,3387,3388,3389,3390,3391,3392
    3393,3394,3395,3396,3397,3398,3399,3400,3401,3402,3403,3404,3405,3406,3407,3408,3409,3410
  },
  { // 28
    1216,1217,1218,1219,1220,1221,1222,1223,1224,1225,1226,1227,1228,1229,1230,1231,1232,1233
    1234,1235,1236,1237,1238,1239,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3385,3384,3383,3382,3381,3380
    3379,3378,3377,3376,3375,3374,3373,3372,3371,3370,3369,3368,3367,3366,3365,3364,3363,3362
  },
  { // 29
    1262,1261,1260,1259,1258,1257,1256,1255,1254,1253,1252,1251,1250,1249,1248,1247,1246,1245
    1244,1243,1242,1241,1240,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3339,3340,3341,3342,3343
    3344,3345,3346,3347,3348,3349,3350,3351,3352,3353,3354,3355,3356,3357,3358,3359,3360,3361
  },
  { // 30
    1263,1264,1265,1266,1267,1268,1269,1270,1271,1272,1273,1274,1275,1276,1277,1278,1279,1280
    1281,1282,1283,1284,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3338,3337,3336,3335
    3334,3333,3332,3331,3330,3329,3328,3327,3326,3325,3324,3323,3322,3321,3320,3319,3318,3317
  },
  { // 31
    1305,1304,1303,1302,1301,1300,1299,1298,1297,1296,1295,1294,1293,1292,1291,1290,1289,1288
    1287,1286,1285,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3296,3297,3298
    3299,3300,3301,3302,3303,3304,3305,3306,3307,3308,3309,3310,3311,3312,3313,3314,3315,3316
  },
  { // 32
    1306,1307,1308,1309,1310,1311,1312,1313,1314,1315,1316,1317,1318,1319,1320,1321,1322,1323
    1324,1325,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3295,3294
    3293,3292,3291,3290,3289,3288,3287,3286,3285,3284,3283,3282,3281,3280,3279,3278,3277,3276
  },
  { // 33
    1344,1343,1342,1341,1340,1339,1338,1337,1336,1335,1334,1333,1332,1331,1330,1329,1328,1327
    1326,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3257
    3258,3259,3260,3261,3262,3263,3264,3265,3266,3267,3268,3269,3270,3271,3272,3273,3274,3275
  },
  { // 34
    1345,1346,1347,1348,1349,1350,1351,1352,1353,1354,1355,1356,1357,1358,1359,1360,1361,1362
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    3256,3255,3254,3253,3252,3251,3250,3249,3248,3247,3246,3245,3244,3243,3242,3241,3240,3239
  },
  { // 35
    1380,1379,1378,1377,1376,1375,1374,1373,1372,1371,1370,1369,1368,1367,1366,1365,1364,1363
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    3221,3222,3223,3224,3225,3226,3227,3228,3229,3230,3231,3232,3233,3234,3235,3236,3237,3238
  },
  { // 36
    1381,1382,1383,1384,1385,1386,1387,1388,1389,1390,1391,1392,1393,1394,1395,1396,1397,1398
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    3220,3219,3218,3217,3216,3215,3214,3213,3212,3211,3210,3209,3208,3207,3206,3205,3204,3203
  },
  { // 37
    1416,1415,1414,1413,1412,1411,1410,1409,1408,1407,1406,1405,1404,1403,1402,1401,1400,1399
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    3185,3186,3187,3188,3189,3190,3191,3192,3193,3194,3195,3196,3197,3198,3199,3200,3201,3202
  },
  { // 38
    1417,1418,1419,1420,1421,1422,1423,1424,1425,1426,1427,1428,1429,1430,1431,1432,1433,1434
    1435,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3184
    3183,3182,3181,3180,3179,3178,3177,3176,3175,3174,3173,3172,3171,3170,3169,3168,3167,3166
  },
  { // 39
    1455,1454,1453,1452,1451,1450,1449,1448,1447,1446,1445,1444,1443,1442,1441,1440,1439,1438
    1437,1436,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3146,3147
    3148,3149,3150,3151,3152,3153,3154,3155,3156,3157,3158,3159,3160,3161,3162,3163,3164,3165
  },
  { // 40
    1456,1457,1458,1459,1460,1461,1462,1463,1464,1465,1466,1467,1468,1469,1470,1471,1472,1473
    1474,1475,1476,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3145,3144,3143
    3142,3141,3140,3139,3138,3137,3136,3135,3134,3133,3132,3131,3130,3129,3128,3127,3126,3125
  },
  { // 41
    1498,1497,1496,1495,1494,1493,1492,1491,1490,1489,1488,1487,1486,1485,1484,1483,1482,1481
    1480,1479,1478,1477,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3103,3104,3105,3106
    3107,3108,3109,3110,3111,3112,3113,3114,3115,3116,3117,3118,3119,3120,3121,3122,3123,3124
  },
  { // 42
    1499,1500,1501,1502,1503,1504,1505,1506,1507,1508,1509,1510,1511,1512,1513,1514,1515,1516
    1517,1518,1519,1520,1521,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3102,3101,3100,3099,3098
    3097,3096,3095,3094,3093,3092,3091,3090,3089,3088,3087,3086,3085,3084,3083,3082,3081,3080
  },
  { // 43
    1545,1544,1543,1542,1541,1540,1539,1538,1537,1536,1535,1534,1533,1532,1531,1530,1529,1528
    1527,1526,1525,1524,1523,1522,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3056,3057,3058,3059,3060,3061
    3062,3063,3064,3065,3066,3067,3068,3069,3070,3071,3072,3073,3074,3075,3076,3077,3078,3079
  },
  { // 44
    1546,1547,1548,1549,1550,1551,1552,1553,1554,1555,1556,1557,1558,1559,1560,1561,1562,1563
    1564,1565,1566,1567,1568,1569,1570,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3055,3054,3053,3052,3051,3050,3049
    3048,3047,3046,3045,3044,3043,3042,3041,3040,3039,3038,3037,3036,3035,3034,3033,3032,3031
  },
  { // 45
    1596,1595,1594,1593,1592,1591,1590,1589,1588,1587,1586,1585,1584,1583,1582,1581,1580,1579
    1578,1577,1576,1575,1574,1573,1572,1571,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,3005,3006,3007,3008,3009,3010,3011,3012
    3013,3014,3015,3016,3017,3018,3019,3020,3021,3022,3023,3024,3025,3026,3027,3028,3029,3030
  },
  { // 46
    1597,1598,1599,1600,1601,1602,1603,1604,1605,1606,1607,1608,1609,1610,1611,1612,1613,1614
    1615,1616,1617,1618,1619,1620,1621,1622,1623,   0,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,   0,3004,3003,3002,3001,3000,2999,2998,2997,2996
    2995,2994,2993,2992,2991,2990,2989,2988,2987,2986,2985,2984,2983,2982,2981,2980,2979,2978
  },
  { // 47
    1651,1650,1649,1648,1647,1646,1645,1644,1643,1642,1641,1640,1639,1638,1637,1636,1635,1634
    1633,1632,1631,1630,1629,1628,1627,1626,1625,1624,   0,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,   0,2950,2951,2952,2953,2954,2955,2956,2957,2958,2959
    2960,2961,2962,2963,2964,2965,2966,2967,2968,2969,2970,2971,2972,2973,2974,2975,2976,2977
  },
  { // 48
    1652,1653,1654,1655,1656,1657,1658,1659,1660,1661,1662,1663,1664,1665,1666,1667,1668,1669
    1670,1671,1672,1673,1674,1675,1676,1677,1678,1679,1680,   0,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,   0,2949,2948,2947,2946,2945,2944,2943,2942,2941,2940,2939
    2938,2937,2936,2935,2934,2933,2932,2931,2930,2929,2928,2927,2926,2925,2924,2923,2922,2921
  },
  { // 49
    1710,1709,1708,1707,1706,1705,1704,1703,1702,1701,1700,1699,1698,1697,1696,1695,1694,1693
    1692,1691,1690,1689,1688,1687,1686,1685,1684,1683,1682,1681,   0,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,   0,2891,2892,2893,2894,2895,2896,2897,2898,2899,2900,2901,2902
    2903,2904,2905,2906,2907,2908,2909,2910,2911,2912,2913,2914,2915,2916,2917,2918,2919,2920
  },
  { // 50
    1711,1712,1713,1714,1715,1716,1717,1718,1719,1720,1721,1722,1723,1724,1725,1726,1727,1728
    1729,1730,1731,1732,1733,1734,1735,1736,1737,1738,1739,1740,1741,   0,   0,   0,   0,   0
       0,   0,   0,   0,   0,2890,2889,2888,2887,2886,2885,2884,2883,2882,2881,2880,2879,2878
    2877,2876,2875,2874,2873,2872,2871,2870,2869,2868,2867,2866,2865,2864,2863,2862,2861,2860
  },
  { // 51
    1773,1772,1771,1770,1769,1768,1767,1766,1765,1764,1763,1762,1761,1760,1759,1758,1757,1756
    1755,1754,1753,1752,1751,1750,1749,1748,1747,1746,1745,1744,1743,1742,   0,   0,   0,   0
       0,   0,   0,   0,2828,2829,2830,2831,2832,2833,2834,2835,2836,2837,2838,2839,2840,2841
    2842,2843,2844,2845,2846,2847,2848,2849,2850,2851,2852,2853,2854,2855,2856,2857,2858,2859
  },
  { // 52
    1774,1775,1776,1777,1778,1779,1780,1781,1782,1783,1784,1785,1786,1787,1788,1789,1790,1791
    1792,1793,1794,1795,1796,1797,1798,1799,1800,1801,1802,1803,1804,1805,1806,   0,   0,   0
       0,   0,   0,2827,2826,2825,2824,2823,2822,2821,2820,2819,2818,2817,2816,2815,2814,2813
    2812,2811,2810,2809,2808,2807,2806,2805,2804,2803,2802,2801,2800,2799,2798,2797,2796,2795
  },
  { // 53
    1840,1839,1838,1837,1836,1835,1834,1833,1832,1831,1830,1829,1828,1827,1826,1825,1824,1823
    1822,1821,1820,1819,1818,1817,1816,1815,1814,1813,1812,1811,1810,1809,1808,1807,   0,   0
       0,   0,2761,2762,2763,2764,2765,2766,2767,2768,2769,2770,2771,2772,2773,2774,2775,2776
    2777,2778,2779,2780,2781,2782,2783,2784,2785,2786,2787,2788,2789,2790,2791,2792,2793,2794
  },
  { // 54
       0,   0,1841,1842,1843,1844,1845,1846,1847,1848,1849,1850,1851,1852,1853,1854,1855,1856
    1857,1858,1859,1860,1861,1862,1863,1864,1865,1866,1867,1868,1869,1870,1871,1872,1873,1874
    1875,1876,1877,1878,1879,1880,1881,1882,1883,1884,1885,1886,1887,1888,1889,1890,1891,1892
    1893,1894,1895,1896,1897,1898,1899,1900,1901,1902,1903,1904,1905,1906,1907,1908,   0,   0
  },
  { // 55
       0,   0,   0,1974,1973,1972,1971,1970,1969,1968,1967,1966,1965,1964,1963,1962,1961,1960
    1959,1958,1957,1956,1955,1954,1953,1952,1951,1950,1949,1948,1947,1946,1945,1944,1943,1942
    1941,1940,1939,1938,1937,1936,1935,1934,1933,1932,1931,1930,1929,1928,1927,1926,1925,1924
    1923,1922,1921,1920,1919,1918,1917,1916,1915,1914,1913,1912,1911,1910,1909,   0,   0,   0
  },
  { // 56
       0,   0,   0,   0,1975,1976,1977,1978,1979,1980,1981,1982,1983,1984,1985,1986,1987,1988
    1989,1990,1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002,2003,2004,2005,2006
    2007,2008,2009,2010,2011,2012,2013,2014,2015,2016,2017,2018,2019,2020,2021,2022,2023,2024
    2025,2026,2027,2028,2029,2030,2031,2032,2033,2034,2035,2036,2037,2038,   0,   0,   0,   0
  },
  { // 57
       0,   0,   0,   0,   0,2100,2099,2098,2097,2096,2095,2094,2093,2092,2091,2090,2089,2088
    2087,2086,2085,2084,2083,2082,2081,2080,2079,2078,2077,2076,2075,2074,2073,2072,2071,2070
    2069,2068,2067,2066,2065,2064,2063,2062,2061,2060,2059,2058,2057,2056,2055,2054,2053,2052
    2051,2050,2049,2048,2047,2046,2045,2044,2043,2042,2041,2040,2039,   0,   0,   0,   0,   0
  },
  { // 58
       0,   0,   0,   0,   0,   0,2101,2102,2103,2104,2105,2106,2107,2108,2109,2110,2111,2112
    2113,2114,2115,2116,2117,2118,2119,2120,2121,2122,2123,2124,2125,2126,2127,2128,2129,2130
    2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142,2143,2144,2145,2146,2147,2148
    2149,2150,2151,2152,2153,2154,2155,2156,2157,2158,2159,2160,   0,   0,   0,   0,   0,   0
  },
  { // 59
       0,   0,   0,   0,   0,   0,   0,2218,2217,2216,2215,2214,2213,2212,2211,2210,2209,2208
    2207,2206,2205,2204,2203,2202,2201,2200,2199,2198,2197,2196,2195,2194,2193,2192,2191,2190
    2189,2188,2187,2186,2185,2184,2183,2182,2181,2180,2179,2178,2177,2176,2175,2174,2173,2172
    2171,2170,2169,2168,2167,2166,2165,2164,2163,2162,2161,   0,   0,   0,   0,   0,   0,   0
  },
  { // 60
       0,   0,   0,   0,   0,   0,   0,   0,2219,2220,2221,2222,2223,2224,2225,2226,2227,2228
    2229,2230,2231,2232,2233,2234,2235,2236,2237,2238,2239,2240,2241,2242,2243,2244,2245,2246
    2247,2248,2249,2250,2251,2252,2253,2254,2255,2256,2257,2258,2259,2260,2261,2262,2263,2264
    2265,2266,2267,2268,2269,2270,2271,2272,2273,2274,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 61
       0,   0,   0,   0,   0,   0,   0,   0,   0,2328,2327,2326,2325,2324,2323,2322,2321,2320
    2319,2318,2317,2316,2315,2314,2313,2312,2311,2310,2309,2308,2307,2306,2305,2304,2303,2302
    2301,2300,2299,2298,2297,2296,2295,2294,2293,2292,2291,2290,2289,2288,2287,2286,2285,2284
    2283,2282,2281,2280,2279,2278,2277,2276,2275,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 62
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2329,2330,2331,2332,2333,2334,2335,2336
    2337,2338,2339,2340,2341,2342,2343,2344,2345,2346,2347,2348,2349,2350,2351,2352,2353,2354
    2355,2356,2357,2358,2359,2360,2361,2362,2363,2364,2365,2366,2367,2368,2369,2370,2371,2372
    2373,2374,2375,2376,2377,2378,2379,2380,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 63
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2430,2429,2428,2427,2426,2425,2424
    2423,2422,2421,2420,2419,2418,2417,2416,2415,2414,2413,2412,2411,2410,2409,2408,2407,2406
    2405,2404,2403,2402,2401,2400,2399,2398,2397,2396,2395,2394,2393,2392,2391,2390,2389,2388
    2387,2386,2385,2384,2383,2382,2381,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 64
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2431,2432,2433,2434,2435,2436
    2437,2438,2439,2440,2441,2442,2443,2444,2445,2446,2447,2448,2449,2450,2451,2452,2453,2454
    2455,2456,2457,2458,2459,2460,2461,2462,2463,2464,2465,2466,2467,2468,2469,2470,2471,2472
    2473,2474,2475,2476,2477,2478,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 65
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2524,2523,2522,2521,2520
    2519,2518,2517,2516,2515,2514,2513,2512,2511,2510,2509,2508,2507,2506,2505,2504,2503,2502
    2501,2500,2499,2498,2497,2496,2495,2494,2493,2492,2491,2490,2489,2488,2487,2486,2485,2484
    2483,2482,2481,2480,2479,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 66
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2525,2526,2527,2528
    2529,2530,2531,2532,2533,2534,2535,2536,2537,2538,2539,2540,2541,2542,2543,2544,2545,2546
    2547,2548,2549,2550,2551,2552,2553,2554,2555,2556,2557,2558,2559,2560,2561,2562,2563,2564
    2565,2566,2567,2568,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 67
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2610,2609,2608
    2607,2606,2605,2604,2603,2602,2601,2600,2599,2598,2597,2596,2595,2594,2593,2592,2591,2590
    2589,2588,2587,2586,2585,2584,2583,2582,2581,2580,2579,2578,2577,2576,2575,2574,2573,2572
    2571,2570,2569,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 68
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2611,2612
    2613,2614,2615,2616,2617,2618,2619,2620,2621,2622,2623,2624,2625,2626,2627,2628,2629,2630
    2631,2632,2633,2634,2635,2636,2637,2638,2639,2640,2641,2642,2643,2644,2645,2646,2647,2648
    2649,2650,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 69
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,2688
    2687,2686,2685,2684,2683,2682,2681,2680,2679,2678,2677,2676,2675,2674,2673,2672,2671,2670
    2669,2668,2667,2666,2665,2664,2663,2662,2661,2660,2659,2658,2657,2656,2655,2654,2653,2652
    2651,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 70
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    2689,2690,2691,2692,2693,2694,2695,2696,2697,2698,2699,2700,2701,2702,2703,2704,2705,2706
    2707,2708,2709,2710,2711,2712,2713,2714,2715,2716,2717,2718,2719,2720,2721,2722,2723,2724
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
  { // 71
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
    2760,2759,2758,2757,2756,2755,2754,2753,2752,2751,2750,2749,2748,2747,2746,2745,2744,2743
    2742,2741,2740,2739,2738,2737,2736,2735,2734,2733,2732,2731,2730,2729,2728,2727,2726,2725
       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  },
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NLEDS, PIN, NEO_GRBW  + NEO_KHZ800); 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off' (that's what the comment says; do I actually do this?)
}

void loop() {
  // your usual BlinkWithoutDelay, to verify that the board is working
  static unsigned long previousMillis = 0;
  static int ledState = LOW;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(LED_BUILTIN, ledState);
  }
  float health = getHealth(); 
  rainbow(health); // could replace with some other pattern
  strip.show();
  // I'm guessing 6 is a magic number that makes the speed work well?
  
}
// get health data from the serial connection...I don't remember whether I did a non-stateless version because I wanted to cache it or because I was lazy.
float getHealth()
{
  if (Serial.available() > 0) {
    char serialChar = Serial.read();
    return byte(serialChar)/255.0;
  } else {
    return 1.0;
  }
}
// change the saturation based on the health of the coral
void bleach(float hlth, uint32_t color)
{
    float bleached = 1.0 - hlth;
    byte r = (color >> 16);
    byte g = (color >>  8);
    byte b = (uint8_t) color;
    return strip.Color(hlth * r, hlth * g, hlth * b, bleached * 255);
}



// *** map an x,y coordinate to a pixel
int xy2pixel(int x, int y)
{
  return grid_map[x][y]; // trivial function once we have the array, but in case we have to change it some day...
}

// our one and only pattern for now
void rainbow(float hlth)
{
  static int counter =   0;
  for (int i=0; i<(4*SIDE); i++)
  {
    for (int j=0; j<(4*SIDE); j++)
    {
      int pixel = xy2pixel(i,j);
      if (pixel >= 0)
      {
        int angle = rainbow_xy2angle(i,j);
        int color = rainbow_angle2color(angle, counter);
        color = bleach(hlth, color);
        strip.setPixelColor(pixel, color);
      }
    }
  }
  counter += 6;
}

// *** map an x,y coordinate to an angle (relative to the center)
double rainbow_xy2angle(int y, int x)
{
  return (atan2(y-2*SIDE, x-2*SIDE)*180)/M_PI;
}

int rainbow_angle2color(int angle, int cntr)
{
  int pos = map((angle + cntr)%360, 0, 360, 0, 255);
  int color = wheel(pos);
  return color;
}

// swiped from the Adafruit test pattern
uint32_t wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}