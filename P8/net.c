#include <stdio.h>
#include <math.h>
#include "user.h"
 
#define MAX_ITERATIONS 6
#define MAX_ERROR 1e-7 

/* global variables */ 
int t_channel = 28;
double lam_h_i = 0.2;
double lam_n = 10;
double mu_r = 0.0167;
double lam_h_o = 0.33;
double lam_d = 0.5;
double lam_f = 0.000016677;
int g_c = 1;
double timeVal; 
 
/* Prototype for the function(s) */ 

double BH ();
double ACh ();
double BN ();
double hotput ();
double ftput ();
double fnum ();


/* ================= OPTIONS ================ */
void options() { 

}

/* ========= DEFINITION OF THE NET ========== */
void net() { 

  /* === PARAM VARIABLES === */
  parm("lam_h_i");
  parm("lam_n");
  /*  ======  PLACE  ======  */ 
  place("T");
  place("B");
  place("R");
  place("CP");
  init("CP",t_channel);

  /*  ======  TRANSITION  ======  */ 
  /* Immediate Transitions */ 
  imm("t1");
  priority("t1",100);
  probval("t1",1.0);
  /* Timed Transitions */ 
  rateval("t_h_i",1.0);
  useparm("t_h_i","lam_h_i");
  rateval("t_n",1.0);
  useparm("t_n","lam_n");
  rateval("t_r",mu_r);
  ratedep("t_h_o",lam_h_o,"T");
  ratedep("t_d",lam_d,"T");
  ratedep("t_f",lam_f,"T");

  /*  ======  ARC ====== */ 
  /* Input Arcs */ 
  iarc("t_f","T");
  iarc("t_h_o","T");
  iarc("t_r","R");
  iarc("t_d","T");
  iarc("t1","R");
  iarc("t1","B");
  iarc("t_h_i","CP");
  miarc("t_n","CP",g_c+1);
  /* Output Arcs */ 
  oarc("t_h_i","T");
  oarc("t_n","T");
  oarc("t_f","B");
  oarc("t_f","R");
  oarc("t_h_o","CP");
  oarc("t_r","CP");
  oarc("t_d","CP");
  oarc("t1","T");
  moarc("t_n","CP",g_c);

  /* === BIND PARAM VARIABLES === */
  bind("lam_h_i",lam_h_i);
  bind("lam_n",lam_n);
} 

/* REWARD Functions */ 
double BH () {
  if (mark("CP")==0)
    return (1.0);
  else 
    return (0.0);

}
double ACh () {
return(mark("CP"));
}
double BN () {
  if (mark("CP")<=g_c)
    return (1.0);
  else 
    return (0.0);
}
double hotput () {
  return(rate("t_h_o"));
}
double ftput () {
  return(rate("t_f"));
}
double fnum () {
  return(mark("B"));
}


/* ======= DEFINITION OF THE FUNCTIONS ====== */

int assert() { 
  return(RES_NOERR);
} 

void ac_init() { 
/* Information on the net structure */ 
pr_net_info();
} 

void ac_reach() { 
/* Information on the reachability graph */ 
pr_rg_info();
} 


void ac_final() { 
int loop; 
  int i;
  double tp,err;
  
  for (i=1; i<MAX_ITERATIONS; i++) {
    pr_value("lam_h_i", lam_h_i);
    bind("lam_h_i", lam_h_i);
    solve(INFINITY);
    tp = expected(hotput);
    pr_value("Throughput of t_h_o", tp);
    err = fabs((lam_h_i-tp)/tp);
    pr_value("Error", err);
    if( err < MAX_ERROR ) break;
    lam_h_i = tp;
  }

  pr_expected("block handoff:", BH);
  pr_expected("block new:", BN);   
  pr_expected("available channel: ", ACh);
  pr_value("avg. waiting time:", expected(fnum)/expected(ftput));
solve(INFINITY); 

pr_expected("Expected reward rate of BH in steady-state", BH); 

} 

