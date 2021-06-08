#include <stdio.h>
#include "user.h"
 
 

/* Prototype for the function(s) */ 

double ef0 ();
double ef1 ();
double ef2 ();
double ef3 ();
double eff ();


/* ================= OPTIONS ================ */
void options() { 

}

/* ========= DEFINITION OF THE NET ========== */
void net() { 
  /*  ======  PLACE  ======  */ 
  place("P0");
  init("P0",1);
  place("P1");
  place("P2");
  place("P3");
  place("P4");

  /*  ======  TRANSITION  ======  */ 
  /* Timed Transitions */ 
  rateval("t0",1.0);
  rateval("t1",3.0);
  rateval("t2",7.0);
  rateval("t3",9.0);
  rateval("t4",5.0);

  /*  ======  ARC ====== */ 
  /* Input Arcs */ 
  iarc("t0","P0");
  iarc("t1","P1");
  iarc("t2","P2");
  iarc("t4","P4");
  iarc("t4","P3");
  iarc("t3","P3");
  /* Output Arcs */ 
  oarc("t0","P1");
  oarc("t0","P2");
  oarc("t1","P3");
  oarc("t2","P4");
  oarc("t4","P0");
  oarc("t3","P1");
} 

/* REWARD Functions */ 
double ef0 () {
return ((double)mark("P0"));

}
double ef1 () {
return ((double)mark("P1"));

}
double ef2 () {
return (rate("t2"));

}
double ef3 () {
return (rate("t3"));

}
double eff () {
return (rate("t1")*1.8+(double)mark("P3")*0.7);

}


/* ======= DEFINITION OF THE FUNCTIONS ====== */

int assert() { 
if (mark("P3") > 5) {  
  return(RES_ERROR);
} else {
  return(RES_NOERR);
}

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
solve(INFINITY);
pr_expected("mark(P0)",ef0);
pr_expected("mark(P1)",ef1);
pr_expected("rate(t2)",ef2);
pr_expected("rate(t3)",ef3);
pr_expected("rate(t1)*1.8+mark(P3)*0.7",eff);
pr_mc_info();
pr_std_average();

solve(INFINITY); 

pr_expected("Expected reward rate of ef0 in steady-state", ef0); 

} 

