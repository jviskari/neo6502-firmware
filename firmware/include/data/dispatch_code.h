//
//	This file is automatically generated
//
switch (*DCOMMAND) {
	case 0:
		switch (*DFUNCTION) {
			case 0:
				DSPReset();
				break;
			case 1:
				*((uint32_t *)DPARAMS) = TMRRead();
				break;
		}
		break;
	case 1:
		switch (*DFUNCTION) {
			case 0:
				CONWrite(*DPARAMS);
				break;
			case 1:
				*(DPARAMS) = KBDGetKey();
				break;
			case 2:
				*(DPARAMS) = KBDIsKeyAvailable() ? 0xFF: 0;
				break;
		}
		break;
}
