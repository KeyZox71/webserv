FROM	alpine:3.21

ADD		./ /build

RUN		apk add --no-cache clang make \
		&& cd /build \
		&& make \
		&& chmod +x webserv \
		&& cp webserv /webserv

RUN		[ "/webserv", "$WEBSERV_CONF"]
