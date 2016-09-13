all:\
	Decoder\
	Endoder\
	PassEncoder\
	PassDecoder

%:%.cpp
	g++ -std=c++14 -o $@ $< \
	mv $@ ..

Encoder:Encoder.cpp
Decoder:Decoder.cpp
PassEncoder:PassEncoder.cpp
PassDecoder:PassDecoder.cpp
