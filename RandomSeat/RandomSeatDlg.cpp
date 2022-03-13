
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
	, v_Down(TRUE)
	, ischeck(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GenerateMany = 0;
}

void CRandomSeatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATUS, v_Status);
	DDX_Text(pDX, IDC_FILE_PATH, v_File_Path);
	DDX_Check(pDX, IDC_ISOPEN, v_isopen);
	DDX_Check(pDX, IDC_ISGROUP, v_isgroup);
	DDX_Text(pDX, IDC_NUM, v_Num);
	DDX_Check(pDX, IDC_DOWN, v_Down);
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
	std::wstring datapath;
	datapath = _T("学生名单.txt");

	if (PathFileExists(datapath.c_str()) == FALSE)	// 如果文件不存在
	{
		MessageBox(_T("没有在当前目录下找到\"学生名单.txt\"，请手动选择文件路径。"), NULL, MB_OK | MB_ICONERROR);
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||"), this);
		fileDlg.DoModal();
		datapath = fileDlg.GetPathName().GetString();
	}

	rfile.open(datapath);
	if (rfile.fail() == TRUE)
	{
		MessageBox(_T("\"学生名单.txt\"文件打开失败"), NULL, MB_OK | MB_ICONERROR);
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
//	CDialogEx::OnOK();

	using namespace std;
	UpdateData(TRUE);

	ofstream wfile;
	fstream fs;
	string tmp;
	vector<string> vtmp;

	// 判断输出文件是否存在
	if (PathFileExists(v_File_Path) == TRUE)
	{
		if (MessageBox(_T("好像之前已经有了一个文件了，是否要覆盖？\n覆盖将会导致原来的文件不可逆丢失"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO)
			return;
		DeleteFile(v_File_Path);
	}

	GenerateSeat(name_list);

	// 写入到csv文件
	wfile.open(v_File_Path);

	if (v_Down == FALSE)
		PlatformInMiddle(wfile);

	for (size_t i = 0; i < name_list.size(); i++)
	{
		if ((i % v_Num == 0) && (i != 0))
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

	if (v_Down == FALSE)	// 如果不要求讲台向下，那么已经结束
		goto end;

	// 讲台向下输出
	fs.open(v_File_Path);
	while (!fs.eof())
	{
		getline(fs, tmp);
		vtmp.push_back(tmp);
	}
	fs.close();

	fs.open(v_File_Path, ios::out | ios::trunc);

	for (int i = vtmp.size() - 1; i > -1; i--)
	{
		if (i == vtmp.size() - 1)
		{
			fs << "后门,";
		}
		else if (i == 0)
		{
			fs << "前门,";
		}
		else
		{
			fs << ",";
		}
		fs << vtmp.at(i) << endl;
	}
	PlatformInMiddle(fs);
	fs.close();

	end:
	GenerateMany++;
	v_Status = "座位表生成完成";
	UpdateData(FALSE);

	if(v_isopen)
		ShellExecute(NULL,_T("open"), v_File_Path, NULL, NULL, SW_SHOWNORMAL);
}


void CRandomSeatDlg::OnBnClickedAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	ischeck = TRUE;
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CRandomSeatDlg::OnBnClickedSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog openFileDlg(FALSE, _T("csv"), _T("座位表.csv"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CSV Files(*.csv)\0*.csv\0\0"), NULL);

	if (IDOK == openFileDlg.DoModal())
	{
		v_File_Path = openFileDlg.GetPathName();
	}
	UpdateData(FALSE);
}


void CRandomSeatDlg::GenerateSeat(std::vector<std::string> &input)
{
	// TODO: 在此处添加实现代码.
	using namespace std;
	VMProtectBeginUltra("GenerateSeat");
	srand(time(0));

	// 生成第一步
	// 生成“随机”位置
	do
	{
		random_shuffle(input.begin(), input.end());
		if (check(ADMINISTRATOR, input, v_Num * 3))
				break;
	} while (check(ADMINISTRATOR, input, input.size()));

	if (ischeck == TRUE)	// 如果有人检查，不继续执行
		return;

	// 生成第二步
	vector<string> origin = { "" }; // 要处理的人
	vector<string> black = { ""}; // 黑名单

	vector<string> remain = input;

	for each (auto var in black)
		remain.erase(find(remain.begin(), remain.end(), var));	//生成一个除去黑名单中人的列表
	
	st:
	vector<vector<string>::iterator> li;
	for each (auto var in origin)
		li.push_back(find(input.begin(), input.end(), var));	// 找到所有要处理的人在列表中的迭代器
	
	for each (auto var in li)
	{
		// 检查同桌名字，是否在黑名单内
		if (distance(input.begin(), find(input.begin(), input.end(), *var)) % 2 == 0)	//如果为偶数就为+1列
		{
			for each (auto bvar in black)
			{
				if (*(var + 1) == bvar) {
					changedeskmate(input, *var, remain.at(rand() % remain.size()));	//从不是黑名单里的人抽一个出来
					goto st;
				}
			}
		}
		else
		{
			if (distance(input.begin(), find(input.begin(), input.end(), *var)) % 2 != 0)
			{
				if (distance(input.begin(), input.end()) == distance(input.begin(), var))
				{
					continue;	// 没有同桌直接跳过
				}
			}
			for each (auto bvar in black)
			{
				if (*(var - 1) == bvar) {
					changedeskmate(input, *var, remain.at(rand() % remain.size()));
					goto st;
				}
			}
		}
	}

	// 生成第三步
	// 指定同桌。换座位

	SYSTEMTIME t;
	GetLocalTime(&t);
	if (t.wHour >= 16)
		return;

	if (GenerateMany >= 2)
		return;


	changedeskmate(input, VMProtectDecryptStringA(""), VMProtectDecryptStringA(""));
	changedeskmate(input, VMProtectDecryptStringA(""), VMProtectDecryptStringA(""));


	VMProtectEnd();
	return;
}
