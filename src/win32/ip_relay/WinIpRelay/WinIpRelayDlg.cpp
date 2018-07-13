// WinIpRelayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinIpRelay.h"
#include "WinIpRelayDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// IP relay includes
#include "RelayDialog.h"
#include "lib_ip_relay.h"
#include "RegistrationDialog.h"
#include <process.h> 

// IP relay defines
#define MAX_STR_BUFFER       1024
#define IPR_FILE_EXT         ".ipr"
#define IPR_FILE_EXT_FILTER  "WinIpRelay files (*.ipr)|*.ipr|All Files (*.*)|*.*||"
#define IPR_DEFAULT_FILENAME "relay_config.ipr"
#define LOG_FILE_EXT         ".txt"
#define LOG_FILE_EXT_FILTER  "Log files (*.txt)|*.txt|All Files (*.*)|*.*||"
#define LOG_DEFAULT_FILENAME "relay_log.txt"
#define IPR_CONFIG_SIGNATURE "IP_RELAY_CONFIG"
#define IPR_CONFIG_VERSION   "1.0"
#define LOCALHOST_IP         0x7f000001
#define LOG_REFRESH_TIMER_ID 007  
#define LOG_REFRESH_PERIOD   1000 // in ms  
#define IPR_STACK_SIZE       (256*1024)
#define IPR_REG_COPYRIGHT    "Copyright (C) 2003 - Voodoo Technologies"

// IP relay node
struct _WIPR_relay_node {
    struct _WIPR_relay_node *next;            // next node 
    struct _WIPR_relay_node *prev;            // prev node 
    struct _WIPR_relay_node **head;           // head node 
    UINT                    local_port;       // local_port
    char                    *remote_host;     // remote host
    UINT                    remote_port;      // remote port
    long                    timeout;          // timeout
    char                    *display_name;    // relay display name
    bool                    dead;             // if TRUE relay thread should be terminated
    CString                 *logs;            // pointer to logs 
    HANDLE                  logs_mutex;       // logs mutex 
    HANDLE                  relay_head_mutex; // relay head mutex
};



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinIpRelayDlg dialog

CWinIpRelayDlg::CWinIpRelayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinIpRelayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinIpRelayDlg)
	m_config_name = _T("");
	m_logs = _T("");
	m_copyright = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32    
    m_hIcon            = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_relay_head       = NULL;
    m_config_changed   = FALSE;
    m_logs_mutex       = CreateMutex( NULL, FALSE, NULL );
    m_relay_head_mutex = CreateMutex( NULL, FALSE, NULL );
    m_logs_size        = 0;
    ipr_class_init();
}

void CWinIpRelayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinIpRelayDlg)
	DDX_Control(pDX, IDC_LOGS, m_logs_ctrl);
	DDX_Control(pDX, IDC_RELAY_LIST, m_relay_list);
	DDX_Text(pDX, IDC_CONFIG_NAME, m_config_name);
	DDX_Text(pDX, IDC_LOGS, m_logs);
	DDV_MaxChars(pDX, m_logs, 2140000000);
	DDX_Text(pDX, IDC_COPYRIGHT, m_copyright);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinIpRelayDlg, CDialog)
	//{{AFX_MSG_MAP(CWinIpRelayDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_RELAY, OnAddRelay)
	ON_BN_CLICKED(IDC_DELETE_RELAY, OnDeleteRelay)
	ON_BN_CLICKED(IDC_SAVE_LOGS, OnSaveLogs)
	ON_BN_CLICKED(IDC_CLEAR_LOGS, OnClearLogs)
	ON_BN_CLICKED(IDC_LOAD_CONFIG, OnLoadConfig)
	ON_BN_CLICKED(IDC_SAVE_CONFIG, OnSaveConfig)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ABOUT_US, OnAboutUs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinIpRelayDlg message handlers

BOOL CWinIpRelayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    m_copyright = _T( IPR_REG_COPYRIGHT );
    UpdateData( FALSE );
    m_logs_ctrl.LimitText( UINT_MAX );  // sets logs size to UINT max
    SetTimer( LOG_REFRESH_TIMER_ID, LOG_REFRESH_PERIOD, NULL );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinIpRelayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinIpRelayDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinIpRelayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWinIpRelayDlg::OnAddRelay() 
{
    CRelayDialog relay_dialog;
    CString      mb_str;
    char         *display_name;


    // displays relay_dialog
    if ( relay_dialog.DoModal() != IDOK ) return;

    // adds relay to relay list
    display_name = AddRelay( relay_dialog.m_local_port, relay_dialog.m_remote_host,
                             relay_dialog.m_remote_port, relay_dialog.m_timeout );
    if ( !display_name ) {
        mb_str.Format( "Could not create new relay localhost:%d <-> %s:%d!",
                        relay_dialog.m_local_port, relay_dialog.m_remote_host, 
                        relay_dialog.m_remote_port );
        MessageBox( mb_str, "Add Relay Error!", MB_OK | MB_ICONERROR );
        return;
    }
    
    // adds relay to relay list
    m_relay_list.AddString( display_name );
    m_relay_list.SetCurSel( m_relay_list.GetCount( ) - 1 );
    free( display_name );

    // sets config changed indicator
    m_config_changed = TRUE;
}

void CWinIpRelayDlg::OnDeleteRelay() 
{
    int     cur_sel;
    CString mb_str;
    char    display_name[MAX_STR_BUFFER];

    // returns here if list is empty
    if ( m_relay_list.GetCount() == 0 || m_relay_list.GetCurSel() < 0 ) return;

    // gets display name of currently focused list item
    cur_sel = m_relay_list.GetCurSel();
    m_relay_list.GetText( cur_sel, display_name );

    // confirms relay deletion
    mb_str = "Do you want to delete relay " + CString( display_name ) + "?";
    if ( MessageBox( mb_str, "Delete Relay?", MB_YESNO | MB_ICONQUESTION ) != IDYES) {
        return;
    }

    // deletes relay
    if ( !DeleteRelay( display_name ) ) {
        mb_str = "Could not delete relay " + CString( display_name ) + "!";
        MessageBox( mb_str, "Delete Relay Error!", MB_OK | MB_ICONERROR );
        return;
    }

    // removes it from relay list 
    m_relay_list.DeleteString( cur_sel );
    if ( cur_sel > 0 ) m_relay_list.SetCurSel( cur_sel - 1 );
    else if ( m_relay_list.GetCount( ) > 0 ) m_relay_list.SetCurSel( 0 );

    // sets config changed indicator
    m_config_changed = TRUE;

}


void CWinIpRelayDlg::OnSaveLogs() 
{
    FILE        *lf;
    CString     mb_str;
    CFileDialog file_dialog( FALSE, LOG_FILE_EXT, LOG_DEFAULT_FILENAME, 
                             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                             LOG_FILE_EXT_FILTER, NULL );

    // displays file dialog
    if ( file_dialog.DoModal() != IDOK ) {
        return;
    }

    // opens log file
    lf = fopen( file_dialog.GetFileName(), "wt" );
    if ( !lf ) {
        mb_str = "Could not create file " + file_dialog.GetFileName() + "!";
        MessageBox( mb_str, "Save Log Error!", MB_OK | MB_ICONERROR );
        return;
    }

    // saves logs
    WaitForSingleObject( m_logs_mutex, INFINITE );
    fprintf( lf, "%s", m_logs.GetBuffer(0) );
    m_logs.ReleaseBuffer( );
    ReleaseMutex( m_logs_mutex );

    // closes log file
    fclose( lf );
}


void CWinIpRelayDlg::OnClearLogs() 
{
    WaitForSingleObject( m_logs_mutex, INFINITE );
    m_logs = _T("");
    ReleaseMutex( m_logs_mutex );
}


// config file format:
//     signature version
//     local_port1 remote_host1 remote_port1 timeout1
//     local_port2 remote_host2 remote_port2 timeout2
//     ...
//     local_portn remote_hostn remote_portn timeoutn
//
void CWinIpRelayDlg::OnLoadConfig() 
{
    int         i;
    UINT        local_port;
    UINT        remote_port;
    long        timeout;
	FILE        *cf;
    char        *new_display_name;
    CString     mb_str;
    char        signature[MAX_STR_BUFFER] = { '\0' };
    char        version[MAX_STR_BUFFER] = { '\0' };
    char        display_name[MAX_STR_BUFFER] = { '\0' };
    char        remote_host[MAX_STR_BUFFER] = { '\0' };

    // if relay list is not empty prompt user first
    if ( m_relay_list.GetCount() > 0 ) {
        if ( MessageBox( "Discard current relay list?", "Load Config", 
                         MB_YESNO | MB_ICONQUESTION ) != IDYES ) 
        {
            return;
        }
    }

    // creates new dialog
    CFileDialog file_dialog( TRUE, IPR_FILE_EXT, IPR_DEFAULT_FILENAME, 
                             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                             IPR_FILE_EXT_FILTER, NULL );

    // displays file dialog
    if ( file_dialog.DoModal() != IDOK ) {
        return;
    }

    // opens config file
    cf = fopen( file_dialog.GetFileName(), "rt" );
    if ( !cf ) {
        mb_str = "Could not read file " + file_dialog.GetFileName() + "!";
        MessageBox( mb_str, "Load Config Error!", MB_OK | MB_ICONERROR );
        return;
    }

    // checks config file signature and version
    fscanf( cf, "%s %s", signature, version );
    if ( strcmp( signature, IPR_CONFIG_SIGNATURE ) || strcmp( version, IPR_CONFIG_VERSION ) ) {
        mb_str = file_dialog.GetFileName() + ": Invalid config format!";
        MessageBox( mb_str, "Load Config Error!", MB_OK | MB_ICONERROR );
        return;
    }

    // deletes all relays
    for ( i = 0; i < m_relay_list.GetCount(); i++ ) {
        m_relay_list.GetText( i, display_name );
        if ( !DeleteRelay( display_name ) ) {
            mb_str = CString( "Could not delete relay " ) + display_name + "!";
            MessageBox( mb_str, "Load Config Error!", MB_OK | MB_ICONERROR );
        }
        m_relay_list.DeleteString( i );
    }

    // reads config and creates relays
    // FORMAT: local_port remote_host remote_port timeout
    while ( fscanf( cf, "%d %s %d %d", &local_port, remote_host, &remote_port, &timeout ) != EOF ) {
        new_display_name = AddRelay( local_port, CString( remote_host ), remote_port, timeout );
        if ( !new_display_name ) {
            mb_str = "Could not create new relay localhost:" + CString( local_port ) +
                     " <-> " + remote_host + ":" + CString( remote_port ) + "!";
            MessageBox( mb_str, "Load Config Error!", MB_OK | MB_ICONERROR );
            return;
        }
        m_relay_list.AddString( new_display_name );
        m_relay_list.SetCurSel( m_relay_list.GetCount( ) - 1 );
        free( new_display_name );        
    }

    // sets config name variable
    m_config_name = file_dialog.GetFileName();

    // closes config file
    fclose( cf );

    // resets config changed indicator
    m_config_changed = FALSE;

}



// config file format:
//     signature version
//     local_port1 remote_host1 remote_port1 timeout1
//     local_port2 remote_host2 remote_port2 timeout2
//     ...
//     local_portn remote_hostn remote_portn timeoutn
//
void CWinIpRelayDlg::OnSaveConfig() 
{
	FILE            *cf;
    WIPR_relay_node *rn;
    CString         mb_str;

    // if relay list is empty exits
    if ( m_relay_list.GetCount() == 0 ) {
        MessageBox( "Nothing to save!", "Save Config", MB_OK ); 
        return;
    }

    // creates new dialog
    CFileDialog file_dialog( FALSE, IPR_FILE_EXT, IPR_DEFAULT_FILENAME, 
                             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                             IPR_FILE_EXT_FILTER, NULL );

    // displays file dialog
    if ( file_dialog.DoModal() != IDOK ) {
        return;
    }

    // opens config file
    cf = fopen( file_dialog.GetFileName(), "wt" );
    if ( !cf ) {
        mb_str = "Could not create file " + file_dialog.GetFileName() + "!";
        MessageBox( mb_str, "Load Config Error!", MB_OK | MB_ICONERROR );
        return;
    }

    // writes config file signature and version
    fprintf( cf, "%s %s\n", IPR_CONFIG_SIGNATURE, IPR_CONFIG_VERSION );

    // writes all relay configs
    // FORMAT: local_port remote_host remote_port timeout
    for ( rn = m_relay_head; rn; rn = rn->next ) {
        fprintf( cf, "%d %s %d %ld\n", rn->local_port, rn->remote_host, rn->remote_port, 
                 rn->timeout );
    }

    // closes config file
    fclose( cf );

    // resets config changed indicator
    m_config_changed = FALSE;
	
}



/* converts IPR_log_level to strings */
static const char *level_to_str( IPR_log_level level )
{
    /* returns level string */
    switch ( level ) {
        case IPR_LL_INFO:
            return "INFO";
        case IPR_LL_WARNING:
            return "WARNING";
        case IPR_LL_ERROR:
            return "ERROR";
        default:
            return "";
    }

    /* returns */
    return "";
}


/* ip_relay log msg function */
static void log_msg( 
    IPR_relay     *relay,       /**< relay logging message */
    IPR_log_level level,        /**< log level */
    const char    *msg,         /**< message to log */
    void          *client_data  /**< client data */ 
)
{
    WIPR_relay_node *rn = (WIPR_relay_node *)client_data;
    char            str[MAX_STR_BUFFER];

    // locks relay list 
    WaitForSingleObject( rn->relay_head_mutex, INFINITE );

    // updates log variable
    WaitForSingleObject( rn->logs_mutex, INFINITE );
    sprintf( str, "localhost:%d - %s - ", rn->local_port, level_to_str( level ) );
    *(rn->logs) += CString( str ) + msg + "\r\n";
    ReleaseMutex( rn->logs_mutex );    

    // unlocks relay list
    ReleaseMutex( rn->relay_head_mutex );
}



/* deletes relay_node */
static void delete_relay_node( WIPR_relay_node *rn )
{
    // locks relay node list 
    WaitForSingleObject( rn->relay_head_mutex, INFINITE );
    
    // removes rn from list 
    if ( rn->prev ) rn->prev->next = rn->next;
    if ( rn->next ) rn->next->prev = rn->prev;
    if ( *(rn->head) == rn ) *(rn->head) = rn->next;    

    // unlocks relay node list 
    ReleaseMutex( rn->relay_head_mutex );

    // deletes allocated objects 
    free( rn->display_name );
    free( rn->remote_host );
    free( rn );
}




/* ip_relay thread function */
static void __cdecl ip_relay_thread( void *args )
{
    int             n_fd_ready;
    IPR_relay       *relay;	
    WIPR_relay_node *rn;
    static fd_set   read_mask;  /* masks moved to BSS to avoid crouding the stack */
    static fd_set   write_mask; /* masks moved to BSS to avoid crouding the stack */
    static fd_set   error_mask; /* masks moved to BSS to avoid crouding the stack */
    struct timeval  timeout;   
    char            str[MAX_STR_BUFFER];

    /* gets relay node */
    rn = (WIPR_relay_node *)args;

    /* creates relay */
    relay = ipr_relay_new( rn->local_port, rn->remote_host, rn->remote_port, rn->timeout, log_msg, rn );
    if ( !relay ) {
        log_msg( relay, IPR_LL_ERROR, "could not create relay!", rn );
        delete_relay_node( rn );
        _endthread();
        return;
    }

    /*  prints running trace */
    sprintf( str, "running on localhost:%d <-> %s:%d", rn->local_port,
             rn->remote_host, rn->remote_port );
    log_msg( relay, IPR_LL_INFO, str, rn );

    /* do forever */
    for ( ; /* ever */ ; ) {

        /* resets fd masks */    
		FD_ZERO( &read_mask );
        FD_ZERO( &write_mask );
        FD_ZERO( &error_mask );
        timeout.tv_sec  = IPR_DEFAULT_TIMEOUT;
        timeout.tv_usec = 0;

		/* sets relay fds  */
		if ( ipr_select_fds_set( relay, &read_mask, &write_mask, &error_mask, &timeout ) < 0 ) {
            log_msg( relay, IPR_LL_ERROR, "could not set relay fd to select masks!", rn );
            break;
		}

        /* sleeps on select */
        n_fd_ready = select( FD_SETSIZE, &read_mask, &write_mask, &error_mask, &timeout );

        /* exits here if relay is dead */
        WaitForSingleObject( rn->relay_head_mutex, INFINITE );
        if ( rn->dead ) {
            ReleaseMutex( rn->relay_head_mutex );
            log_msg( relay, IPR_LL_INFO, "relay killed. cleaning up...", rn );
            break;
        }
        ReleaseMutex( rn->relay_head_mutex );

        /* ignores select errors */
        if ( n_fd_ready < 0 ) {
            log_msg( relay, IPR_LL_WARNING, "select failed! ignoring...", rn );	
            continue;
        }

        /* checks relay fds */
        if ( ipr_select_fds_check( relay, &read_mask, &write_mask, &error_mask, n_fd_ready ) < 0 ) {
            log_msg( relay, IPR_LL_ERROR, "could not check relay fds!", rn );
            break;
		}
	}

    /* deletes relay  */    
    ipr_relay_delete( relay );

    // deletes relay node
    delete_relay_node( rn );

    // end threads
    _endthread();
}


char *CWinIpRelayDlg::AddRelay( UINT local_port, CString& remote_host, UINT remote_port, long timeout ) 
{
    char            display_name[MAX_STR_BUFFER];
    WIPR_relay_node *rn;

    // checks if local_port is already in use
    WaitForSingleObject( m_relay_head_mutex, INFINITE );    
    for ( rn = m_relay_head; rn && rn->local_port != local_port; rn = rn->next );
    ReleaseMutex( m_relay_head_mutex );
    if ( rn ) {
        sprintf( display_name, "Port %d already in use!", local_port );
        MessageBox( display_name, "Add Relay Error!", MB_OK | MB_ICONERROR );
        return NULL;
    }

    // builds display_name
    sprintf( display_name, "localhost:%d <-> %.511s:%d", local_port, remote_host.GetBuffer(0), remote_port );
    remote_host.ReleaseBuffer( );

    // creates new relay node
    rn = new WIPR_relay_node;
    if ( !rn ) {
        MessageBox( "Out of Memory!", "Add Relay Error!", MB_OK | MB_ICONERROR );
        return NULL;
    }
    rn->dead             = FALSE;
    rn->local_port       = local_port;
    rn->remote_host      = strdup( remote_host );
    rn->remote_port      = remote_port;
    rn->timeout          = timeout;
    rn->display_name     = strdup( display_name );
    rn->logs             = &m_logs;
    rn->logs_mutex       = m_logs_mutex;
    rn->relay_head_mutex = m_relay_head_mutex;
    rn->head             = &m_relay_head;

    // sets relay node list head to rn
    WaitForSingleObject( m_relay_head_mutex, INFINITE );    
    rn->prev = NULL;
    rn->next = m_relay_head;
    if ( m_relay_head ) m_relay_head->prev = rn;
    m_relay_head = rn;
    ReleaseMutex( m_relay_head_mutex );

    // starts relay thread
    if  ( _beginthread( ip_relay_thread, IPR_STACK_SIZE, rn ) < 0 ) {
        MessageBox( "Could not create relay thread!", "Add Relay Error!", MB_OK | MB_ICONERROR );
        delete_relay_node( rn );
        return NULL;
    }

    // updates logs 
    WaitForSingleObject( m_logs_mutex, INFINITE );
    m_logs += CString( "WinIpRelay - INFO - created relay " ) + display_name + "\r\n";
    ReleaseMutex( m_logs_mutex );    

    // returns
    return strdup( display_name );	
}



/* signals relay to wake-up relay out of select */
static BOOL signal_relay( WIPR_relay_node *rn )
{
    SOCKET signal_socket;
	struct sockaddr_in sock_addr;

    // gets signal ip and port
    WaitForSingleObject( rn->relay_head_mutex, INFINITE );
    int  port = rn->local_port;
    long ip   = LOCALHOST_IP;
    ReleaseMutex( rn->relay_head_mutex );

    /* creates signal sockets */
	signal_socket = socket( AF_INET, SOCK_STREAM, 0 ); 
	if ( signal_socket == INVALID_SOCKET ) {		
		return FALSE;
	}

    /* connects signal sockets */
    memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = htonl( ip );
	sock_addr.sin_port        = htons( (unsigned short)port );
    connect( signal_socket, (struct sockaddr *)&sock_addr, sizeof(sock_addr) );

    /* close signal socket */
    closesocket( signal_socket );

    // returns success
    return TRUE;
}



BOOL CWinIpRelayDlg::DeleteRelay( const char *display_name ) 
{
    WIPR_relay_node *rn;

    // locates relay node
    WaitForSingleObject( m_relay_head_mutex, INFINITE );    
    for ( rn = m_relay_head; rn; rn = rn->next ) {
        if ( !strcmp( rn->display_name, display_name ) ) break;
    }    

    // exits here if relay was not found in the list 
    if ( !rn ) {
        ReleaseMutex( m_relay_head_mutex );
        return FALSE;
    }

    // sets dead flag
    rn->dead = TRUE;
    ReleaseMutex( m_relay_head_mutex );

    // signals relay
    signal_relay( rn );

    //returns
    return TRUE;	
}


void CWinIpRelayDlg::OnTimer(UINT nIDEvent) 
{
	// updates logs control if needed
    if ( nIDEvent == LOG_REFRESH_TIMER_ID ) {
        WaitForSingleObject( m_logs_mutex, INFINITE );
        if ( m_logs_size != m_logs.GetLength() ) {
            m_logs_size = m_logs.GetLength();
            UpdateData( FALSE );
        }
        ReleaseMutex( m_logs_mutex );
        SetTimer( LOG_REFRESH_TIMER_ID, LOG_REFRESH_PERIOD, NULL );
    }

	CDialog::OnTimer(nIDEvent);
}


void CWinIpRelayDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    ipr_class_delete();
}


void CWinIpRelayDlg::OnOK() 
{
    // if relay list is not empty prompt user first
    if ( m_relay_list.GetCount() > 0 ) {
        if ( MessageBox( "Delete current relay(s) and exit?", "Exit Confirmation", 
                         MB_YESNO | MB_ICONQUESTION ) != IDYES ) 
        {
            return;
        }
    }
    
    // deletes all relays
    for ( int i = 0; i < m_relay_list.GetCount(); i++ ) {
        char display_name[MAX_STR_BUFFER];
        m_relay_list.GetText( i, display_name );
        if ( !DeleteRelay( display_name ) ) {
            CString mb_str = CString( "Could not delete relay " ) + display_name + "!";
            MessageBox( mb_str, "Exit Error!", MB_OK | MB_ICONERROR );
        }
        m_relay_list.DeleteString( i );
    }
	
	// call base class
	CDialog::OnOK();
}


void CWinIpRelayDlg::OnAboutUs() 
{
    // just displays about dialog
    CAboutDlg dlgAbout;
	dlgAbout.DoModal();	   
}
