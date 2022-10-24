FROM aflplusplus/aflplusplus:4.02c

RUN apt-get -y update && apt-get -y upgrade
RUN apt-get install binutils-dev libunwind-dev clang curl pkg-config libipt-dev libunwind8-dev libncurses5 zip unzip golang -y
RUN apt update ; apt install autopoint libtool gettext libpopt-dev -y
RUN apt-get update
RUN apt-get install gnat-11 -y
WORKDIR /home/

RUN git clone https://github.com/google/honggfuzz.git
WORKDIR /home/honggfuzz
RUN make #&& cp /home/honggfuzz/honggfuzz /bin

WORKDIR /home/
RUN mv /AFLplusplus/ /home/
RUN git clone https://github.com/google/fuzzing.git libfuzz
RUN git clone https://github.com/google/fuzzer-test-suite.git FTS
RUN sed -i 's/sudo//' ./libfuzz/tutorial/libFuzzer/install-*
RUN ./libfuzz/tutorial/libFuzzer/install-*  # Get deps

RUN mkdir $HOME/src/
RUN git clone https://github.com/jfoote/exploitable.git $HOME/src/exploitable
WORKDIR /root/src/exploitable/
RUN python setup.py install
RUN go install github.com/bnagy/crashwalk/cmd/cwtriage@latest && go install github.com/bnagy/crashwalk/cmd/cwdump@latest && go install github.com/bnagy/crashwalk/cmd/cwfind@latest
ENV PATH=$PATH:/root/go/bin:/usr/lib/gcc/x86_64-linux-gnu/11/:/home/honggfuzz/hfuzz_cc:/home/honggfuzz/
WORKDIR /home/
