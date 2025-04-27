FROM		alpine:3.21

COPY		./ /build

ENV			PKGS=true
ENV			TTY=true

RUN			apk add --no-cache bash clang make

RUN			cd /build \
			&& PKGS=true make -j re

COPY		/build/webserv /bin/webserv

RUN			chmod +x /bin/webserv

WORKDIR		/

RUN			webserv --generate

STOPSIGNAL	SIGINT

RUN			[ "webserv", "/sample.toml"]
