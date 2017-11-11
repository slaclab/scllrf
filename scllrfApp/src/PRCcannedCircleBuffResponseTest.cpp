	cout << "Now processing " << __FILE__ << endl;

	FpgaReg cannedCircleData[8193] =
	{
#include "CircleBuffData.csv"
	};

	for ( unsigned int i = 0; i< 8193; i++)
	{
		cout << "Fake data read: addr 0x" << std::hex << cannedCircleData[i].addr << ", data: 0x" << cannedCircleData[i].data << endl;
		if (cannedCircleData[i].addr & flagReadMask)
		{
			processRegReadback(&cannedCircleData[i], dummy);
		}
		else
		{
			processRegWriteResponse(&cannedCircleData[i]);
		}
	}
	lastResponseCount_ += 1;
