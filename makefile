all:
	gcc stripe.c -ggdb -lcurl -ostripe

clean:
	rm stripe
