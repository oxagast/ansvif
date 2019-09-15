FROM ubuntu:16.04

WORKDIR /src

RUN apt update && apt-get --no-install-recommends -yqq install \
	ca-certificates \
	git \
	automake \
	autoconf-archive \
	zlib1g-dev \
	g++ \
	libgcc-4.8-dev \
	gcc \
	gtk2.0

RUN git clone https://github.com/oxagast/ansvif.git
WORKDIR ansvif
RUN	aclocal
RUN autoconf
RUN automake -a
RUN ./configure --disable-gtk
RUN make


CMD ./ansvif -t examples/specific/flag_chars.txt -c ./generic_buffer_overflow -b 64 -z -L root
