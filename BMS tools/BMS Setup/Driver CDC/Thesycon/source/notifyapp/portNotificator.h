/************************************************************************
 *
 *  Module:       portNotificator.h
 *  Description:
 *     a special notificator class called if a virtual com port device is added or removed
 *
 *  Author(s):   
 *    Guenter Hildebrandt
 *                
 *  Companies:
 *    Thesycon GmbH, Germany      http://www.thesycon.de
 *                
 ************************************************************************/

#ifndef __portNotificator_h__
#define __portNotificator_h__

class PortNotificator: public CPnPNotifyHandler, public CPnPNotificator
{
public:
	virtual 
	void 
	HandlePnPMessage(
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		);

};


#endif  // __portNotificator_h__

/*************************** EOF **************************************/
