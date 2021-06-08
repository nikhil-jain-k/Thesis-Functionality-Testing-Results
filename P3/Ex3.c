#include <stdio.h>
#include "user.h"
 
 

/* global variables */ 
double lambda = 0.0067;
int b = 7;
double mu = 0.0056;
int m = 5;
double timeVal; 
 
/* Prototype for the function(s) */ 

double rate_serv ();
double qlength ();
double util ();
double tput ();
double probrej ();
double probempty ();
double probhalffull ();


/* ================= OPTIONS ================ */
void options() { 

}

/* ========= DEFINITION OF THE NET ========== */
void net() { 
  /*  ======  PLACE  ======  */ 
  place("buf");

  /*  ======  TRANSITION  ======  */ 
  /* Timed Transitions */ 
  rateval("trin",lambda);
  ratefun("trserv",rate_serv);

  /*  ======  ARC ====== */ 
  /* Input Arcs */ 
  iarc("trserv","buf");
  /* Output Arcs */ 
  oarc("trin","buf");
  /* Inhibtor Arcs */ 
  mharc("trin","buf",b);
} 

/* DISTRIBUTION Functions */ 
double rate_serv () {
/* Marking dependent firing rate */
if(mark("buf")<m)
  return (mark("buf")*mu);
else
  return(m*mu);
}


/* REWARD Functions */ 
double qlength () {
return(mark("buf"));
}
double util () {
return(enabled("trserv"));
}
double tput () {
return(rate("trserv"));
}
double probrej () {
if ( mark("buf") == b ) 
      return(1.0);
else 
      return(0.0); 

}
double probempty () {
if ( mark("buf") == 0 ) 
    return(1.0);
else 
    return(0.0); 

}
double probhalffull () {
if ( mark("buf") == b/2 ) 
    return(1.0);
else 
    return(0.0); 

}


/* ======= DEFINITION OF THE FUNCTIONS ====== */

int assert() { 
/* Make sure that the number of tokens in buf does not exceed the buffer size */
if(mark("buf")>b)
  return(RES_ERROR);
else
  return(RES_NOERR);
} 

void ac_init() { 
fprintf(stderr,"A model of the M/M/m/b Queue");
pr_net_info();
} 

void ac_reach() { 
pr_rg_info();
} 


void ac_final() { 
int loop; 
   double time_pt;

   /* measures related to the queue */
   solve(INFINITY);
   pr_expected("Average Queue Length", qlength);
   pr_expected("Average Throughput", tput);
   pr_expected("Utilization", util);
	  
   /* this case corresponds to buf having b tokens */
   pr_expected("Probability of rejection",probrej);
	  
   /* this case corresponds to buf having zero tokens */
   pr_expected("Probability that queue is empty",probempty);
	  
   /* this case corresponds to buf having b/2 tokens */
   pr_expected("Probability that queue is half full",probhalffull);
	
   
    for ( time_pt = 0.1; time_pt < 1.0; time_pt += 0.1) {
        solve(time_pt);
        pr_expected("Average Queue Length", qlength);
        pr_expected("Average Throughput", tput);
        pr_expected("Utilization", util);


        /* this case corresponds to buf having b tokens */
        pr_expected("Probability of rejection",probrej);

        /* this case corresponds to buf having zero tokens */
        pr_expected("Probability that queue is empty",probempty);

        /* this case corresponds to buf having b/2 tokens */
        pr_expected("Probability that queue is half full",probhalffull);
    }

    for ( time_pt = 1.0; time_pt < 10.0; time_pt += 1.0) {
        solve(time_pt);
        pr_expected("Average Queue Length", qlength);
        pr_expected("Average Throughput", tput);
        pr_expected("Utilization", util);

        /* this case corresponds to buf having b tokens */
        pr_expected("Probability of rejection",probrej);

        /* this case corresponds to buf having zero tokens */
        pr_expected("Probability that queue is empty",probempty);

        /* this case corresponds to buf having b/2 tokens */
        pr_expected("Probability that queue is half full",probhalffull);
    }

solve(INFINITY); 

pr_expected("Expected reward rate of qlength in steady-state", qlength); 

} 

