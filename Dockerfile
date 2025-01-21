FROM	alpine:3.21

COPY	./ /build

RUN		apk add --no-cache clang make \
		&& cd /build \
		&& make \
		&& chmod +x webserv \
		&& cp webserv /bin/webserv

RUN		[ "/bin/webserv", "$WEBSERV-CONF"]
