/************************************************************************
 *
 *  Module:       PnPNotificator.h
 *  Long name:    PnPNotificator class definition
 *  Description:  implements Win32 device PnP notifications
 *
 *  Runtime Env.: Win32
 *  Author(s):    Frank Senf, Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _PnPNotificator_h_
#define _PnPNotificator_h_


/*doc CPnPNotifyHandler
\begin{class}{}{ class}{CPnPNotifyHandler}{
  The \temph{CPnPNotifyHandler} class defines an interface that is used by the 
  \hl{CPnPNotificator} class to deliver device PnP notifications.
  This interface needs to be implemented by a derived class in order to receive
  Plug\&Play (PnP) notifications.

  Because it defines an interface, the class is an abstract base class.
  }
*/
//
// CPnPNotifyHandler
//
// Abstract base class that defines the interface to be implemented
// to receive notifications from CPnPNotificator.
//
class CPnPNotifyHandler
{
public:

  // called if a WM_DEVICECHANGE is issued by the system
  virtual 
  void 
  HandlePnPMessage(
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    ) = 0;
/*doc
\begin{function}{}{}{virtual void}{HandlePnPMessage}{ = 0}{
  This function is called by \hl{CPnPNotificator} if a \temph{WM_DEVICECHANGE} message
  is issued by the system for one of the registered device interface classes.
  }
\param{UINT}{uMsg}{}{
  The \temph{uMsg} parameter passed to the \ttt{WindowProc} function.
  This parameter is set to \temph{WM_DEVICECHANGE}.
  See the documentation of \temph{WM_DEVICECHANGE} in the Windows Platform SDK for more information.
  }
\param{WPARAM}{wParam}{}{
  The \temph{wParam} parameter passed to the \ttt{WindowProc} function.
  See the documentation of \temph{WM_DEVICECHANGE} for more information.
  }
\param{LPARAM}{lParam}{}{
  The \temph{lParam} parameter passed to the \ttt{WindowProc} function.
  See the documentation of \temph{WM_DEVICECHANGE} for more information.
  }
\comments{
  This function must be implemented by a class that is derived from \temph{CPnPNotifyHandler}.
  A \hl{CPnPNotificator} object calls this function in the context of 
  its internal worker thread when the system issued a \temph{WM_DEVICECHANGE} message
  for one of the device interface classes registered with \hl{CPnPNotificator::EnableDeviceNotifications}.

  \tbold{Caution:} MFC is not aware of the internal worker thread created by a \hl{CPnPNotificator} instance.
  Consequently, no MFC objects should be touched in the context of this function.
  Furthermore, the implementation of \temph{HandlePnPMessage} has to care about proper code synchronization
  when accessing data structures.
  }
\seealso{
  \sa{CPnPNotificator}\\
  \sa{CPnPNotificator::Initialize}\\
  \sa{CPnPNotificator::EnableDeviceNotifications}
  }
\end{function}
*/

}; //class CPnPNotifyHandler
/*doc
\end{class}
*/



/*doc CPnPNotificator
\pagebreak
\begin{class}{}{ class}{CPnPNotificator}{
  This class implements a worker thread that uses
  a hidden window to receive device Plug\&Play (PnP) notification messages 
	(\temph{WM_DEVICECHANGE}) issued by the system.
  }
*/
//
// CPnPNotificator
//
// This class implements a worker thread that uses a hidden 
// window to receive WM_DEVICECHANGE messages issued by the system.
//
class CPnPNotificator
{
public:
  // constructor
  CPnPNotificator();
/*doc
\begin{function}{}{}{}{CPnPNotificator}{}{
  Constructs a CPnPNotificator object.
  }
\end{function}
*/
  // destructor
  ~CPnPNotificator();
/*doc
\begin{function}{}{}{}{\~{}CPnPNotificator}{}{
  Destroys the CPnPNotificator object.
  }
\end{function}
\pagebreak
*/


////////////////////////////////////////////////////////////
// interface
////////////////////////////////////////////////////////////
public:

  //
  // Initialize the CPnPNotificator object. This creates and starts
  // the internal worker thread. The worker thread will register a new
  // window class and create a hidden window that will receive all 
  // PnP notification messages.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  Initialize(
    HINSTANCE hInstance,
    CPnPNotifyHandler* NotifyHandler
    );
/*doc
\begin{function}{}{}{bool}{Initialize}{}{
  Initializes the \temph{CPnPNotificator} object, creates and starts the internal worker thread.
  }
\param{HINSTANCE}{hInstance}{}{
  Provides an instance handle that identifies the owner of the hidden window to be created.
  In a DLL, specify the \temph{hInstance} value passed to \ttt{DllMain}.
  In an executable, provide the \temph{hInstance} value passed to \ttt{WinMain}.
	In a console application use \temph{::GetModuleHandle(NULL)} to obtain the instance handle.
  }
\param{CPnPNotifyHandler*}{NotifyHandler}{}{
  Points to a caller-provided object that implements the \hl{CPnPNotifyHandler} interface.
  This object will receive notifications issued by this \temph{CPnPNotificator} instance.
  }
\retval{
  The function returns true if successful, false otherwise.
  }
\comments{
  The function creates an internal worker thread that will register a window class
  and create a hidden window. 
  The system will post a \temph{WM_DEVICECHANGE} message to this window if a Plug\&Play
  event is detected for any of the registered device interface classes 
  (see \hl{CPnPNotificator::EnableDeviceNotifications}).
  The message will be retrieved by the worker thread and the thread calls 
  \hl{CPnPNotificator::HandlePnPMessage} passing the message parameters unmodified.
  The object that will receive the \hl{CPnPNotifyHandler::HandlePnPMessage} calls is 
  given in \temph{NotifyHandler}.
  This object needs to be derived from \hl{CPnPNotifyHandler} and implements the function 
  \hl{CPnPNotifyHandler::HandlePnPMessage}.

  Note that a call to \temph{Initialize} will initialize the worker thread only.
  In order to receive PnP notifications, \hl{CPnPNotificator::EnableDeviceNotifications} 
  needs to be called at least once.

  The function fails if it is called twice for the same object.
  }
\seealso{
  \sa{CPnPNotificator::Shutdown}\\
  \sa{CPnPNotificator::EnableDeviceNotifications}\\
  \sa{CPnPNotificator::DisableDeviceNotifications}\\
  \sa{CPnPNotifyHandler::HandlePnPMessage}
  }
\end{function}
\pagebreak
*/

  //
  // Shutdown the CPnPNotificator object. This will terminate the 
  // internal worker thread and destroy the hidden window.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  Shutdown(void);
/*doc
\begin{function}{}{}{bool}{Shutdown}{}{
  Terminates the internal worker thread and frees resources.
  }
\retval{
  The function returns true if successful, false otherwise.
  }
\comments{
  This function terminates the internal worker thread, destroys the hidden window
  and frees all resources allocated by \hl{CPnPNotificator::Initialize}.
  A call to this function will also delete all PnP notifications registered with 
  \hl{CPnPNotificator::EnableDeviceNotifications}.

  It is safe to call this function if the object is not initialized.
  The function succeeds in this case.
  }
\seealso{
  \sa{CPnPNotificator::Initialize}\\
  \sa{CPnPNotificator::EnableDeviceNotifications}\\
  \sa{CPnPNotificator::DisableDeviceNotifications}
  }
\end{function}
\pagebreak
*/


  //
  // Enable notifications for a particular class of device interfaces.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  EnableDeviceNotifications(
    const GUID& InterfaceClassGuid
    );
/*doc
\begin{function}{}{}{bool}{EnableDeviceNotifications}{}{
  Enables notifications for a given class of device interfaces.
  }
\param{const GUID\&}{InterfaceClassGuid}{}{
  Specifies the class of device interfaces which will be registered with the system to
  post PnP notifications to this object.
  }
\retval{
  The function returns true if successful, false otherwise.
  }
\comments{
  Call this function once for each device interface class that should be
  registered by this \temph{CPnPNotificator} object.
  When a PnP event occurs for one of the registered interface classes then the 
  operating system posts a \temph{WM_DEVICECHANGE} message to this object
  and the object calls \hl{CPnPNotifyHandler::HandlePnPMessage} in the context of its
  internal worker thread.

  \hl{CPnPNotificator::Initialize} needs to be called before this function can be used.
  }
\seealso{
  \sa{CPnPNotificator::Initialize}\\
  \sa{CPnPNotificator::DisableDeviceNotifications}\\
  \sa{CPnPNotifyHandler::HandlePnPMessage}
  }
\end{function}
\pagebreak
*/

  //
  // Disable notifications for a particular class of device interfaces.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  DisableDeviceNotifications(
    const GUID& InterfaceClassGuid
    );
/*doc
\begin{function}{}{}{bool}{DisableDeviceNotifications}{}{
  Disables notifications for a given class of device interfaces.
  }
\param{const GUID\&}{InterfaceClassGuid}{}{
  Specifies the class of device interfaces which will be unregistered so that no further
  PnP notifications will be posted to this object.
  }
\retval{
  The function returns true if successful, false otherwise.
  }
\comments{
  After this call the \temph{CPnPNotificator} object will stop to issue 
  \hl{CPnPNotifyHandler::HandlePnPMessage} calls for the specified device interface class.

  It is safe to call this function if no notifications are currently registered for the 
  specified device interface class.
	The function succeeds in this case.

  A call to \hl{CPnPNotificator::Shutdown} will disable all device notifications that are
  currently registered.
  }
\seealso{
  \sa{CPnPNotificator::EnableDeviceNotifications}\\
  \sa{CPnPNotificator::Shutdown}
  }
\end{function}
\pagebreak
*/



////////////////////////////////////////////////////////////
// implementation
////////////////////////////////////////////////////////////
protected:

  // this object stores a device notification that has been registered
  struct NotificationRecord 
  {
    // device interface class
    GUID InterfaceClassGuid;
    // handle returned by RegisterDeviceNotification
    HDEVNOTIFY hDevNotify;
    // pointer to next element
    NotificationRecord* Next;

    // constructor
    NotificationRecord()
      {
        ZeroMemory(this, sizeof(*this));
      }
  };


  // delete all notification records in mNotificationList
  void
  DeleteAllNotificationRecords(void);

  //
  // Register a new window class and create a hidden window.
  // Returns true if successful, false otherwise.
  //
  bool
  RegisterAndCreateHiddenWindow(void);

  //
  // Unregister our window class.
  // Returns true if successful, false otherwise.
  //
  bool
  UnregisterWindowClass(void);

  //
  // Register the device interface type identified by Notification.InterfaceClassGuid 
  // so that our internal hidden window will receive notifications.
  // Returns true if successful, false otherwise.
  //
  bool
  RegisterNotification(
    NotificationRecord* Notification
    );

  //
  // Unregister the specified interface type
  // Returns true if successful, false otherwise.
  //
  bool
  UnregisterNotification(
    NotificationRecord* Notification
    );


  // thread routine called by C runtime
  static
  unsigned int
  __stdcall 
  beginthread_routine(
    void* StartContext
    );

  // worker thread main routine
  unsigned int
  ThreadRoutine(void);

  // window proc routine
  static 
  LRESULT 
  CALLBACK
  WndProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    );

  // window proc implementation
  // declared virtual in order to allow derived classes
  // to modify the behavior
  virtual
  LRESULT
  WindowProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    );

  // issue a PnP notification
  void
  IssuePnPNotification(
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    );


////////////////////////////////////////////////////////////
// member variables
protected:

  // critical section that protects the object
  CRITICAL_SECTION mCritSect;

  // state 
  bool mInitialized;

  // single-linked list that holds all notification records currently active
  NotificationRecord* mNotificationList;
  
  // system thread object
  HANDLE mThreadHandle;

  // pointer to notify handler that will receive notifications
  CPnPNotifyHandler* volatile mNotifyHandler;

  // owner of the window
  volatile HINSTANCE mWndOwner;

  // handle of hidden window
  volatile HWND mhWnd;

  // event signaled by worker thread if initialization is finished
  volatile HANDLE mThreadReadyEvent;

};
/*doc
\end{class}
*/


#endif // _PnPNotificator_h_

/*************************** EOF **************************************/
