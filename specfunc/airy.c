/* Author:  G. Jungman
 * RCS:     $Id$ 
 */
#include <stdio.h>
#include <math.h>
#include <gsl_errno.h>
#include <gsl_math.h>
#include "gsl_sf_chebyshev.h"
#include "gsl_sf_airy.h"


/* chebyshev expansions for Airy modulus and phase
   based on SLATEC r9aimp()

 Series for AM21       on the interval -1.25000D-01 to  0.
                                        with weighted error   2.89E-17
                                         log weighted error  16.54
                               significant figures required  14.15
                                    decimal places required  17.34

 Series for ATH1       on the interval -1.25000D-01 to  0.
                                        with weighted error   2.53E-17
                                         log weighted error  16.60
                               significant figures required  15.15
                                    decimal places required  17.38

 Series for AM22       on the interval -1.00000D+00 to -1.25000D-01
                                        with weighted error   2.99E-17
                                         log weighted error  16.52
                               significant figures required  14.57
                                    decimal places required  17.28

 Series for ATH2       on the interval -1.00000D+00 to -1.25000D-01
                                        with weighted error   2.57E-17
                                         log weighted error  16.59
                               significant figures required  15.07
                                    decimal places required  17.34
*/

static double am21_data[40] = {
  .0065809191761485,
  .0023675984685722,
  .0001324741670371,
  .0000157600904043,
  .0000027529702663,
  .0000006102679017,
  .0000001595088468,
  .0000000471033947,
  .0000000152933871,
  .0000000053590722,
  .0000000020000910,
  .0000000007872292,
  .0000000003243103,
  .0000000001390106,
  .0000000000617011,
  .0000000000282491,
  .0000000000132979,
  .0000000000064188,
  .0000000000031697,
  .0000000000015981,
  .0000000000008213,
  .0000000000004296,
  .0000000000002284,
  .0000000000001232,
  .0000000000000675,
  .0000000000000374,
  .0000000000000210,
  .0000000000000119,
  .0000000000000068,
  .0000000000000039,
  .0000000000000023,
  .0000000000000013,
  .0000000000000008,
  .0000000000000005,
  .0000000000000003,
  .0000000000000001,
  .0000000000000001,
  .0000000000000000,
  .0000000000000000,
  .0000000000000000
};

static double ath1_data[36] = {
  -.07125837815669365,
  -.00590471979831451,
  -.00012114544069499,
  -.00000988608542270,
  -.00000138084097352,
  -.00000026142640172,
  -.00000006050432589,
  -.00000001618436223,
  -.00000000483464911,
  -.00000000157655272,
  -.00000000055231518,
  -.00000000020545441,
  -.00000000008043412,
  -.00000000003291252,
  -.00000000001399875,
  -.00000000000616151,
  -.00000000000279614,
  -.00000000000130428,
  -.00000000000062373,
  -.00000000000030512,
  -.00000000000015239,
  -.00000000000007758,
  -.00000000000004020,
  -.00000000000002117,
  -.00000000000001132,
  -.00000000000000614,
  -.00000000000000337,
  -.00000000000000188,
  -.00000000000000105,
  -.00000000000000060,
  -.00000000000000034,
  -.00000000000000020,
  -.00000000000000011,
  -.00000000000000007,
  -.00000000000000004,
  -.00000000000000002
};


static double am22_data[33] = {
 -.01562844480625341,
  .00778336445239681,
  .00086705777047718,
  .00015696627315611,
  .00003563962571432,
  .00000924598335425,
  .00000262110161850,
  .00000079188221651,
  .00000025104152792,
  .00000008265223206,
  .00000002805711662,
  .00000000976821090,
  .00000000347407923,
  .00000000125828132,
  .00000000046298826,
  .00000000017272825,
  .00000000006523192,
  .00000000002490471,
  .00000000000960156,
  .00000000000373448,
  .00000000000146417,
  .00000000000057826,
  .00000000000022991,
  .00000000000009197,
  .00000000000003700,
  .00000000000001496,
  .00000000000000608,
  .00000000000000248,
  .00000000000000101,
  .00000000000000041,
  .00000000000000017,
  .00000000000000007,
  .00000000000000002
};


static double ath2_data[32] = {
   .00440527345871877,
  -.03042919452318455,
  -.00138565328377179,
  -.00018044439089549,
  -.00003380847108327,
  -.00000767818353522,
  -.00000196783944371,
  -.00000054837271158,
  -.00000016254615505,
  -.00000005053049981,
  -.00000001631580701,
  -.00000000543420411,
  -.00000000185739855,
  -.00000000064895120,
  -.00000000023105948,
  -.00000000008363282,
  -.00000000003071196,
  -.00000000001142367,
  -.00000000000429811,
  -.00000000000163389,
  -.00000000000062693,
  -.00000000000024260,
  -.00000000000009461,
  -.00000000000003716,
  -.00000000000001469,
  -.00000000000000584,
  -.00000000000000233,
  -.00000000000000093,
  -.00000000000000037,
  -.00000000000000015,
  -.00000000000000006,
  -.00000000000000002
};


static struct gsl_sf_ChebSeries am21_cs = {
  am21_data,
  39,
  -1, 1
};

static struct gsl_sf_ChebSeries ath1_cs = {
  ath1_data,
  35,
  -1, 1
};

static struct gsl_sf_ChebSeries am22_cs = {
  am22_data,
  32,
  -1, 1
};

static struct gsl_sf_ChebSeries ath2_cs = {
  ath2_data,
  31,
  -1, 1
};



/* Airy modulus and phase for x < -1 */
static void airy_mod_phase(double x, double * mod, double * phase)
{
  static double pi4 = 0.25 * M_PI;
  double sqx;

  if(x < -2.0) {
    double z = 16.0/(x*x*x) + 1.0;
    *mod   = 0.3125 + gsl_sf_cheb_eval(z, &am21_cs);
    *phase = -0.625 + gsl_sf_cheb_eval(z, &ath1_cs);
  }
  else if(x <= -1.0) {
    double z = (16.0/(x*x*x) + 9.0)/7.0;
    *mod   = 0.3125 + gsl_sf_cheb_eval(z, &am22_cs);
    *phase = -0.625 + gsl_sf_cheb_eval(z, &ath2_cs);
  }
  else {
    char buff[50];
    sprintf(buff,"airy_mod_phase: x= %g  > -1", x);
    GSL_MESSAGE(buff);
    *mod = 0.;
    *phase = 0.;
    return;
  }

  sqx = sqrt(-x);
  *mod = sqrt(*mod/sqx);
  *phase = pi4 - x*sqx * *phase;
}



/* Chebyshev series for Ai(x) with x in [-1,1]
   based on SLATEC ai(x)

 series for aif        on the interval -1.00000d+00 to  1.00000d+00
                                   with weighted error   1.09e-19
                                    log weighted error  18.96
                          significant figures required  17.76
                               decimal places required  19.44

 series for aig        on the interval -1.00000d+00 to  1.00000d+00
                                   with weighted error   1.51e-17
                                    log weighted error  16.82
                          significant figures required  15.19
                               decimal places required  17.27
 */
static double ai_data_f[9] = {
  -.03797135849666999750,
   .05919188853726363857,
   .00098629280577279975,
   .00000684884381907656,
   .00000002594202596219,
   .00000000006176612774,
   .00000000000010092454,
   .00000000000000012014,
   .00000000000000000010
};

static double ai_data_g[8] = {
   .01815236558116127,
   .02157256316601076,
   .00025678356987483,
   .00000142652141197,
   .00000000457211492,
   .00000000000952517,
   .00000000000001392,
   .00000000000000001
};

static struct gsl_sf_ChebSeries aif_cs = {
  ai_data_f,
  8,
  -1, 1
};

static struct gsl_sf_ChebSeries aig_cs = {
  ai_data_g,
  7,
  -1, 1
};


/* Chebvyshev series for Bi(x) with x in [-1,1]
   based on SLATEC bi(x)

 series for bif        on the interval -1.00000d+00 to  1.00000d+00
                                        with weighted error   1.88e-19
                                         log weighted error  18.72
                               significant figures required  17.74
                                    decimal places required  19.20

 series for big        on the interval -1.00000d+00 to  1.00000d+00
                                        with weighted error   2.61e-17
                                         log weighted error  16.58
                               significant figures required  15.17
                                    decimal places required  17.03
 */
static double data_bif[9] = {
  -.01673021647198664948,
   .10252335834249445610,
   .00170830925073815165,
   .00001186254546774468,
   .00000004493290701779,
   .00000000010698207143,
   .00000000000017480643,
   .00000000000000020810,
   .00000000000000000018
};
   
static double data_big[8] = {
   .02246622324857452,
   .03736477545301955,
   .00044476218957212,
   .00000247080756363,
   .00000000791913533,
   .00000000001649807,
   .00000000000002411,
   .00000000000000002
};

/* Chebyshev series for Bi(x) with x in [1,8]
   based on SLATEC bi(x)
 */
static double data_bif2[10] = {
  .0998457269381604100,
  .4786249778630055380,
  .0251552119604330118,
  .0005820693885232645,
  .0000074997659644377,
  .0000000613460287034,
  .0000000003462753885,
  .0000000000014288910,
  .0000000000000044962,
  .0000000000000000111
};
static double data_big2[10] = {
  .033305662145514340,
  .161309215123197068,
  .0063190073096134286,
  .0001187904568162517,
  .0000013045345886200,
  .0000000093741259955,
  .0000000000474580188,
  .0000000000001783107,
  .0000000000000005167,
  .0000000000000000011
};

static struct gsl_sf_ChebSeries bif_cs = {
  data_bif,
  8,
  -1, 1
};
static struct gsl_sf_ChebSeries big_cs = {
  data_big,
  7,
  -1, 1
};

static struct gsl_sf_ChebSeries bif2_cs = {
  data_bif2,
  9,
  -1, 1
};
static struct gsl_sf_ChebSeries big2_cs = {
  data_big2,
  9,
  -1, 1
};


/* chebyshev for Ai(x) asymptotic factor 
   based on SLATEC aie()

 Series for AIP        on the interval  0.          to  1.00000D+00
                   with weighted error   5.10E-17
                    log weighted error  16.29
          significant figures required  14.41
               decimal places required  17.06
*/
static double data_aip[34] = {
  -.0187519297793868
  -.0091443848250055,
   .0009010457337825,
  -.0001394184127221,
   .0000273815815785,
  -.0000062750421119,
   .0000016064844184,
  -.0000004476392158,
   .0000001334635874,
  -.0000000420735334,
   .0000000139021990,
  -.0000000047831848,
   .0000000017047897,
  -.0000000006268389,
   .0000000002369824,
  -.0000000000918641,
   .0000000000364278,
  -.0000000000147475,
   .0000000000060851,
  -.0000000000025552,
   .0000000000010906,
  -.0000000000004725,
   .0000000000002076,
  -.0000000000000924,
   .0000000000000417,
  -.0000000000000190,
   .0000000000000087,
  -.0000000000000040,
   .0000000000000019,
  -.0000000000000009,
   .0000000000000004,
  -.0000000000000002,
   .0000000000000001,
  -.0000000000000000
};

/* chebyshev for Bi(x) asymptotic factor 
   based on SLATEC bie()

 Series for BIP        on the interval  1.25000D-01 to  3.53553D-01
                   with weighted error   1.91E-17
                    log weighted error  16.72
          significant figures required  15.35
               decimal places required  17.41

 Series for BIP2       on the interval  0.          to  1.25000D-01
                   with weighted error   1.05E-18
                    log weighted error  17.98
          significant figures required  16.74
               decimal places required  18.71
*/
static double data_bip[24] = {
  -.08322047477943447,
   .01146118927371174,
   .00042896440718911,
  -.00014906639379950,
  -.00001307659726787,
   .00000632759839610,
  -.00000042226696982,
  -.00000019147186298,
   .00000006453106284,
  -.00000000784485467,
  -.00000000096077216,
   .00000000070004713,
  -.00000000017731789,
   .00000000002272089,
   .00000000000165404,
  -.00000000000185171,
   .00000000000059576,
  -.00000000000012194,
   .00000000000001334,
   .00000000000000172,
  -.00000000000000145,
   .00000000000000049,
  -.00000000000000011,
   .00000000000000001
};

static double data_bip2[29] = {    
  -.113596737585988679,
   .0041381473947881595,
   .0001353470622119332,
   .0000104273166530153,
   .0000013474954767849,
   .0000001696537405438,
  -.0000000100965008656,
  -.0000000167291194937,
  -.0000000045815364485,
   .0000000003736681366,
   .0000000005766930320,
   .0000000000621812650,
  -.0000000000632941202,
  -.0000000000149150479,
   .0000000000078896213,
   .0000000000024960513,
  -.0000000000012130075,
  -.0000000000003740493,
   .0000000000002237727,
   .0000000000000474902,
  -.0000000000000452616,
  -.0000000000000030172,
   .0000000000000091058,
  -.0000000000000009814,
  -.0000000000000016429,
   .0000000000000005533,
   .0000000000000002175,
  -.0000000000000001737,
  -.0000000000000000010
};


static struct gsl_sf_ChebSeries cs_aip = {
  data_aip,
  33,
  -1, 1
};

static struct gsl_sf_ChebSeries cs_bip = {
  data_bip,
  23,
  -1, 1
};

static struct gsl_sf_ChebSeries cs_bip2 = {
  data_bip2,
  28,
  -1, 1
};
   

static double airy_aie(double x)
{
  if(x < 1.0) {
    char buff[50];
    sprintf(buff,"airy_aie: x= %g  < 1", x);
    GSL_MESSAGE(buff);
    return 0.;
  }
  else {
    double sqx = sqrt(x);
    double z = 2.0/(x*sqx) - 1.0;
    return (.28125 + gsl_sf_cheb_eval(z, &cs_aip))/sqrt(sqx);
  }
}

static double airy_bie(double x)
{
  static double ATR =  8.7506905708484345;
  static double BTR = -2.093836321356054;

  if(x < 2.0) {
    char buff[50];
    sprintf(buff,"airy_bie: x= %g  < 2", x);
    GSL_MESSAGE(buff);
    return 0.;
  }
  else if(x < 4.0) {
    double sqx = sqrt(x);
    double z   = ATR/(x*sqx) + BTR;
    return (0.625 + gsl_sf_cheb_eval(z, &cs_bip))/sqrt(sqx);
  }
  else {
    double sqx = sqrt(x);
    double z = 16.0/(x*sqx) - 1.0;
    return (0.625 + gsl_sf_cheb_eval(z, &cs_bip2))/sqrt(sqx);
  }
}


double gsl_sf_airy_Ai(double x)
{
  if(x < -1.0) {
    double mod, theta;
    airy_mod_phase(x, &mod, &theta);
    return mod * cos(theta);
  }
  else if(x <= 1.0) {
    double z = x*x*x;
    return 0.375 + (gsl_sf_cheb_eval(z, &aif_cs) - x*(0.25 + gsl_sf_cheb_eval(z, &aig_cs)));
  }
  else {
    return airy_aie(x) * exp(-2.0*x*sqrt(x)/3.0);
  }
}


double gsl_sf_airy_Bi(double x)
{
  if(x < -1.0) {
    double mod, theta;
    airy_mod_phase(x, &mod, &theta);
    return mod * sin(theta);
  }
  else if(x < 1.0) {
    double z = x*x*x;
    return 0.625 + gsl_sf_cheb_eval(z, &bif_cs) + x*(0.4375 + gsl_sf_cheb_eval(z, &big_cs));
  }
  else if(x <= 2.) {
    double z = (2.0*x*x*x - 9.0)/7.0;
    return 1.125 + gsl_sf_cheb_eval(z, &bif2_cs) + x*(0.625 + gsl_sf_cheb_eval(z, &big2_cs));
  }
  else {
    return airy_bie(x) * exp(2.0*x*sqrt(x)/3.0);
  }
}


double gsl_sf_airy_Bi_scaled(double x)
{
  if(x <= 0.) {
    return gsl_sf_airy_Bi(x);
  }
  else if(x < 1.0) {
    double scale = exp(2.0*x*sqrt(x)/3.0);
    double z = x*x*x;
    double bi= 0.625 + gsl_sf_cheb_eval(z,&bif_cs) + x*(0.4375 + gsl_sf_cheb_eval(z,&big_cs));
    return bi/scale;
  }
  else if(x <= 2.) {
    double scale = exp(2.0*x*sqrt(x)/3.0);
    double z = (2.0*x*x*x - 9.0)/7.0;
    double bi =  1.125 +gsl_sf_cheb_eval(z,&bif2_cs) +x*(0.625+gsl_sf_cheb_eval(z,&big2_cs));
    return bi/scale;
  }
  else {
    return airy_bie(x);
  }
}
