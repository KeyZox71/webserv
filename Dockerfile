FROM		alpine:3.21

COPY		./ /build

RUN			apk add --no-cache clang make \
			&& cd /build \
			&& make \
			&& chmod +x webserv \
			&& cp webserv /bin/webserv

STOPSIGNAL	SIGINT

RUN			[ "/bin/webserv", "$WEBSERV-CONF"]
