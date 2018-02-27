	cout << "Now processing " << __FILE__ << endl;

	FpgaReg cannedTraceData[8193] =
	{
#include "GunTraceData.csv"
	};

	for ( unsigned int i = 0; i< 8193; i++)
	{
		cout << "Fake data read: addr 0x" << std::hex << cannedTraceData[i].addr << ", data: 0x" << cannedTraceData[i].data << std::dec << endl;
		if (cannedTraceData[i].addr & flagReadMask)
		{
			processRegReadback(&cannedTraceData[i], dummy);
		}
		else
		{
			processRegWriteResponse(&cannedTraceData[i]);
		}
	}
	lastResponseCount_ += 1;
