// usbhidiocDlg.h : header file
//

typedef enum {
    Pickit2,
    Pickit3,
    pkob,
    PK2M
} PickitType_t;
extern PickitType_t deviceType;

typedef enum
{
    notWritten,
    writeSuccesful,
    writeTimeout
} PickitWriteStatus_t;
extern PickitWriteStatus_t writeStatus;

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg dialog

class CUsbhidioc //: public CDialog
{
public:
	CUsbhidioc(void);
    PickitType_t type() const { return m_type; }
    PickitWriteStatus_t wrStatus() const { return m_wrStatus; }
    char* GetPK2UnitID(void);
	bool FindTheHID(int unitIndex);
    bool ReadReport (char *);
    bool WriteReport(char *, unsigned int);
    void CloseReport ();

protected:
    void GetDeviceCapabilities();
    void PrepareForOverlappedTransfer();

	char m_UnitID[32];
    PickitType_t m_type = Pickit2;
    PickitWriteStatus_t m_wrStatus = notWritten;

};


