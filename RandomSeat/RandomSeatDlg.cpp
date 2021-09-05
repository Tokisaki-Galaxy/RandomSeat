
// RandomSeatDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RandomSeat.h"
#include "RandomSeatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRandomSeatDlg 对话框



CRandomSeatDlg::CRandomSeatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RANDOMSEAT_DIALOG, pParent)
	, v_Status(_T(""))
	, v_File_Path(strcat(_getcwd(nullptr,256) ,"\\座位表.csv"))
	, v_isopen(TRUE)
	, v_isgroup(TRUE)
	, v_Num(8)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRandomSeatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATUS, v_Status);
	DDX_Text(pDX, IDC_FILE_PATH, v_File_Path);
	DDX_Check(pDX, IDC_ISOPEN, v_isopen);
	DDX_Check(pDX, IDC_ISGROUP, v_isgroup);
	DDX_Text(pDX, IDC_NUM, v_Num);
}

BEGIN_MESSAGE_MAP(CRandomSeatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRandomSeatDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ABOUT, &CRandomSeatDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_SELECT, &CRandomSeatDlg::OnBnClickedSelect)
END_MESSAGE_MAP()


// CRandomSeatDlg 消息处理程序

BOOL CRandomSeatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	std::ifstream rfile;
	std::string tmp;
	rfile.open(_T("学生名单.txt"));
	if (rfile.fail())
	{
		CWnd::MessageBox(L"学生名单文件打开失败");
		exit(1);
	}
	while (!rfile.eof())
	{
		std::getline(rfile, tmp);
		name_list.push_back(tmp);
	}
	rfile.close();

	v_Status = "成功读取了学生名单";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRandomSeatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRandomSeatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRandomSeatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRandomSeatDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

	using namespace std;
	UpdateData(TRUE);

	// 判断输出文件是否存在
	if (PathFileExists(v_File_Path))
	{
		if (MessageBox(_T("好像之前已经有了一个文件了，是否要覆盖？\n覆盖将会导致原来的文件不可逆丢失"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO)
			return;
		DeleteFile(v_File_Path);
	}

	// 生成“随机”位置
	srand(time(0));
	do {
		random_shuffle(name_list.begin(), name_list.end());
	} while ((check(name_list, v_Num * 2) && (!check(name_list, name_list.size()))));

	// 写入到csv文件
	ofstream wfile;
	wfile.open(v_File_Path);
	wfile << "讲台" << endl;
	for (size_t i = 0; i < name_list.size(); i++)
	{
		if ((i % v_Num == 0)&&(i!=0))
			wfile << endl;
		if (v_isgroup)
		{
			if ((i % 2 == 0) && (i != 0))
			{
				if (i % v_Num != 0)
				{
					wfile << " ,";
				}
			}
		}
		wfile << name_list.at(i) << ",";
	}
	wfile.close();
	v_Status = "座位表生成完成";
	UpdateData(FALSE);

	if(v_isopen)
		ShellExecute(NULL,_T("open"), v_File_Path, NULL, NULL, SW_SHOWNORMAL);
}


void CRandomSeatDlg::OnBnClickedAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CRandomSeatDlg::OnBnClickedSelect()
{
	// TODO: 在此添加控件通知处理程序代码
// TODO: 在此添加控件通知处理程序代码
	BOOL isOpen = FALSE;					//是否打开(否则为保存)
	CString defaultEx = L"csv";				//默认扩展名
	CString fileName = L"座位表.csv";			//默认文件名
	CString filter = L"CSV Files(*.csv)\0*.csv\0\0";	//文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultEx, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);

	if (IDOK == openFileDlg.DoModal())
	{
		v_File_Path = openFileDlg.GetPathName();
	}
	UpdateData(FALSE);
}


inline int CRandomSeatDlg::check(std::vector<std::string> a,int flag= 8 * 2)	//找人，找到返回0，没找到返回1
{
	// TODO: 在此处添加实现代码.
	for (size_t i = 0; i < flag; i++)
	{
		if (a.at(i) == "xxx")
		{
			return 0;
		}
	}
	return 1;
}
