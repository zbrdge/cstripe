/*
 *   Stripe API using libcURL in plain old C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "methods.h"

// Your API keys here:
#define APISECRET "sk_test_Hml0AfXSV4jOy9vyq1yhxlzY"
#define APIPUBLIC "pk_test_BiszSiwu9JUBbZuUMwMjhZ4A"

int main() {

  char *endpoint = strdup ("https://api.stripe.com");

  CURL *curl;

  curl = stripe_init(APISECRET ":\0");

  curl = stripe_charges( "amount=400&currency=usd&card={}&description=Charge%20for%20test@example.com", curl );

  stripe_close( stripe_exec( curl ) ); 
  
  return 0;
}
