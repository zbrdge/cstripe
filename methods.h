#include <curl/curl.h>


#define APIVERSION	"v1"
#define BASE_ENDPOINT	"https://api.stripe.com"
#define CHARGES		APIVERSION"/charges"
#define COUPONS		APIVERSION"/coupons"
#define CUSTOMERS	APIVERSION"/customers"
#define INVOICES	APIVERSION"/invoices"
#define ITEMS		APIVERSION"/invoiceitems"
#define PLANS		APIVERSION"/plans"
#define TOKENS		APIVERSION"/tokens"
#define EVENTS		APIVERSION"/events"

#define DEBUG

// libcURL boilerplate [see: http://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string]
struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}


/*****************************************************
 *
 *	Stripe API Methods
 *
 *****************************************************/

// set up curl and authenticate
CURL * stripe_init (const char *apikey) {

	CURL *curl;

	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_USERPWD, apikey);
	}	

	return curl;
}


// POST stuff to the CHARGES endpoint
CURL * stripe_charges (const char *pf, CURL *curl) {

	const char *url = BASE_ENDPOINT "/" CHARGES;

	#ifdef DEBUG
	 printf("USING URL: %s\n",url);
	#endif

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pf);
	}
	
	return curl;
}


// POST stuff to the TOKENS endpoint
CURL * stripe_tokens (const char *pf, CURL *curl) {

	const char *url = BASE_ENDPOINT "/" TOKENS;

	#ifdef DEBUG
	 printf("USING URL: %s\n",url);
	#endif

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pf);
	}
	
	return curl;
}


// "execute" the API call
CURL * stripe_exec (CURL *curl) {

	CURLcode res;
	struct string s;

	init_string(&s);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

	res = curl_easy_perform(curl);

	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
		  curl_easy_strerror(res));


	/* XXX: Print the JSON for now */	
	printf("%s\n", s.ptr);
	free(s.ptr);

	return curl;
}


void stripe_close (CURL *curl) {
	curl_easy_cleanup(curl);
}
