FROM		alpine:3.21 AS builder

COPY		./ /build

ENV			PKGS=true
ENV			TTY=true

RUN			apk add --no-cache bash clang make

RUN			cd /build \
			&& PKGS=true make -j re

FROM		alpine:3.21

RUN			apk add --no-cache libstdc++

COPY		--from=builder /build/webserv /bin/webserv

RUN			chmod +x /bin/webserv

WORKDIR		/

RUN			/bin/webserv --generate

STOPSIGNAL	SIGINT

CMD			[ "webserv", "/sample.toml"]
