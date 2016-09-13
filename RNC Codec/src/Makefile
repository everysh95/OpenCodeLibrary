all:\
	Encoder\
	Decoder\
	PassEncoder\
	PassDecoder
	mv $^ ..

%:%.cpp
	${CXX} -std=c++11 -o $@ $^
#	g++-6 -std=c++14 -o $@ $< \
	

Encoder:Encoder.cpp
Decoder:Decoder.cpp
PassEncoder:PassEncoder.cpp
PassDecoder:PassDecoder.cpp
