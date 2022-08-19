FROM aflplusplus/aflplusplus:4.02c

RUN apt-get -y update && apt-get -y upgrade
RUN apt-get install binutils-dev libunwind-dev clang curl -y
WORKDIR /home/
RUN git clone https://github.com/google/honggfuzz.git
RUN mv /AFLplusplus/ /home/

RUN git clone https://github.com/google/fuzzing.git libfuzz
RUN git clone https://github.com/google/fuzzer-test-suite.git FTS
RUN sed -i 's/sudo//' ./libfuzz/tutorial/libFuzzer/install-*
RUN ./libfuzz/tutorial/libFuzzer/install-*  # Get deps





 
