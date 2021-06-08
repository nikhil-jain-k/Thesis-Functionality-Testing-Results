#include <stdio.h>
#include "user.h"
 
 

/* global variables */ 
int n = 1;
int c = 0.95;
double gamma = 0.00000278;
double delta = 6;
double beta = 0.2;
double tao = 0.01667;
int negC;
double timeVal; 
 
/* Prototype for the function(s) */ 

double rw ();
int gf ();


/* ================= OPTIONS ================ */
void options() { 

  negC = 1-c;
}

/* ========= DEFINITION OF THE NET ========== */
void net() { 
  /*  ======  PLACE  ======  */ 
  place("Pup");
  init("Pup",n);
  place("Pfail");
  place("Pcov");
  place("Puncov");
  place("Prep");

  /*  ======  TRANSITION  ======  */ 
  /* Immediate Transitions */ 
  imm("tquick");
  guard("tquick",gf);
  priority("tquick",10);
  probval("tquick",1);
  imm("tcov");
  priority("tcov",1);
  probval("tcov",0.95);
  imm("tuncov");
  priority("tuncov",1);
  probval("tuncov",0.05);
  /* Timed Transitions */ 
  ratedep("Tfail",gamma,"Pup");
  rateval("Trep",tao);
  rateval("Treboot",beta);
  rateval("Treconfig",delta);

  /*  ======  ARC ====== */ 
  /* Input Arcs */ 
  iarc("Tfail","Pup");
  iarc("tcov","Pfail");
  iarc("tuncov","Pfail");
  iarc("Treconfig","Pcov");
  iarc("Treboot","Puncov");
  iarc("Trep","Prep");
  iarc("tquick","Pfail");
  /* Output Arcs */ 
  oarc("Tfail","Pfail");
  oarc("tcov","Pcov");
  oarc("Treconfig","Prep");
  oarc("Treboot","Prep");
  oarc("Trep","Pup");
  oarc("tquick","Prep");
  oarc("tuncov","Puncov");
  /* Inhibtor Arcs */ 
  harc("Tfail","Pcov");
  harc("Tfail","Puncov");
} 

/* GUARD Functions */ 
int gf () {
if(mark("Pup") > 0)
  return 0;
else
  return 1;
}


/* REWARD Functions */ 
double rw () {
if(mark("Pup") >= 1 && (mark("Pcov")+mark("Puncov")==0))
  return 1;
else
  return 0;
}


/* ======= DEFINITION OF THE FUNCTIONS ====== */

int assert() { 

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

pr_expected("Expected reward rate of rw in steady-state", rw); 

} 

