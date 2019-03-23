// DlgModelManager.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "MainFrm.h"
#include "DlgModelManager.h"
#include "NAppDll.h"
#include "DlgModelName.h"
#include "afxdialogex.h"

extern CMainFrame *m_pMainFrm;

/////////////////////////////////////////////////////////////////////////////
// CDlgModelManager dialog
CDlgModelManager::CDlgModelManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModelManager::IDD, pParent)
	, m_editPriorityNum(_T(""))
{
	//{{AFX_DATA_INIT(CDlgModelManager)
	m_sModelName = _T("");
	m_editPriorityNum = _T("");
	//}}AFX_DATA_INIT
}

void CDlgModelManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModelManager)
	DDX_Control(pDX, IDC_STATIC_FRAME, m_staticFrame);
	DDX_Control(pDX, IDC_LIST_MODEL, m_ctrlModel);
	DDX_Text(pDX, IDC_EDIT_MODELNAME, m_sModelName);
	DDX_Control(pDX, IDC_CHECK_AUTO_RECIP, m_btnAutoRecip);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);
	DDX_Text(pDX, IDC_EDIT_RECIPE, m_editPriorityNum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgModelManager, CDialogEx)
	//{{AFX_MSG_MAP(CDlgModelManager)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_LBN_DBLCLK(IDC_LIST_MODEL, OnDblclkListModel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, OnButtonSaveas)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_CHECK_AUTO_RECIP, OnBnClickedCheckAutoRecip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModelManager message handlers
BOOL CDlgModelManager::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	CRect rcRect;
	
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_MODEL), true);
	FillListBoxWithFileName();

	m_btnAutoRecip.SetIcon(IDI_ICON_LARGE_ON, IDI_ICON_LARGE_OFF);
	m_btnAutoRecip.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);
	m_btnAutoRecip.DrawBorder(FALSE);
	m_btnAutoRecip.DrawTransparent();

	m_btnApply.SetIcon(IDI_ICON_APPLY2);
	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnApply.DrawBorder(FALSE);
	m_btnApply.DrawTransparent();

    m_staticFrame.GetWindowRect(&rcRect);
	m_ImageBMP.Create(rcRect.Width(), rcRect.Height(), 3*8);

	UpdateModelName();
	SetTransLanguage(m_pMainFrm->m_Setting.m_nSelLanguage);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModelManager::SetTransLanguage(long nSel)
{
	if (nSel==0)
	{
		SetWindowText(_T(" 모델관리 화면 "));
		GetDlgItem(IDC_STATIC_CURR_MODEL)->SetWindowText(_T("사용중 모델명"));		
		GetDlgItem(IDC_BUTTON_NEW)->SetWindowText(_T("신규모델"));
	    GetDlgItem(IDC_BUTTON_SELECT)->SetWindowText(_T("모델선택"));
	    GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(_T("모델저장"));
	    GetDlgItem(IDC_BUTTON_SAVEAS)->SetWindowText(_T("다른 모델명으로 저장 "));
		GetDlgItem(IDC_BUTTON_RENAME)->SetWindowText(_T("모델명 변경"));
		GetDlgItem(IDC_BUTTON_DELETE)->SetWindowText(_T("모델삭제"));
		GetDlgItem(IDOK)->SetWindowText(_T("종료"));
		GetDlgItem(IDC_CHECK_AUTO_RECIP)->SetWindowText(_T("외부모델변경"));
		GetDlgItem(IDC_BUTTON_APPLY)->SetWindowText(_T("적용"));
		GetDlgItem(IDC_STATIC_PRIORITY)->SetWindowText(_T("우선순위"));
	}
	if (nSel==1)
	{
		SetWindowText(_T(" MODEL MANAGER "));
		GetDlgItem(IDC_STATIC_CURR_MODEL)->SetWindowText(_T("Current Model Name"));
		GetDlgItem(IDC_BUTTON_NEW)->SetWindowText(_T("NEW MODEL"));
	    GetDlgItem(IDC_BUTTON_SELECT)->SetWindowText(_T("SELECT MODEL"));
	    GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(_T("SAVE MODEL"));
	    GetDlgItem(IDC_BUTTON_SAVEAS)->SetWindowText(_T("SAVE AS.. MODEL"));
		GetDlgItem(IDC_BUTTON_RENAME)->SetWindowText(_T("MODIFY MODEL"));
		GetDlgItem(IDC_BUTTON_DELETE)->SetWindowText(_T("DELETE MODEL"));
		GetDlgItem(IDOK)->SetWindowText(_T("EXIT"));
		GetDlgItem(IDC_CHECK_AUTO_RECIP)->SetWindowText(_T("Remote Recipe"));	
		GetDlgItem(IDC_BUTTON_APPLY)->SetWindowText(_T("APPLY"));
		GetDlgItem(IDC_STATIC_PRIORITY)->SetWindowText(_T("Priority Num."));
	}
	if (nSel==2)
	{
		SetWindowText(_T(" 模特管理画面 "));
		GetDlgItem(IDC_STATIC_CURR_MODEL)->SetWindowText(_T("目前模特名"));
		GetDlgItem(IDC_BUTTON_NEW)->SetWindowText(_T("新模特"));
	    GetDlgItem(IDC_BUTTON_SELECT)->SetWindowText(_T("模特选择"));
	    GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(_T("模特保存"));
	    GetDlgItem(IDC_BUTTON_SAVEAS)->SetWindowText(_T("另存为，其它MODEL名"));
		GetDlgItem(IDC_BUTTON_RENAME)->SetWindowText(_T("模特名 变更"));
		GetDlgItem(IDC_BUTTON_DELETE)->SetWindowText(_T("模特删除"));
		GetDlgItem(IDOK)->SetWindowText(_T("结束"));
		GetDlgItem(IDC_CHECK_AUTO_RECIP)->SetWindowText(_T("Remote Recipe"));	
		GetDlgItem(IDC_BUTTON_APPLY)->SetWindowText(_T("适用"));
		GetDlgItem(IDC_STATIC_PRIORITY)->SetWindowText(_T("优先次序"));
	}
}

void CDlgModelManager::FillListBoxWithFileName()
{
	CFileFind cff;
	long nSel;
    CString str;

	/////////////////////////// 읽을 파일의 path를 지정한다.>>>>>>
	TCHAR FileName[128];
	_stprintf_s(FileName,_T("%s\\*.*"), MODEL_FILE_PATH);
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	if (!cff.FindFile(FileName,0)) return; // FindFile이 함수를 실행한 후에야 										   // FindNextFile 함수등을 쓸수 있다.	
	
	while(cff.FindNextFile())
    {
		str=cff.GetFileName();
	    if (cff.IsDirectory() && str.Find(_T("."))<0) 
		{
			m_ctrlModel.AddString(GetRecipModel(str));
			nSel = m_ctrlModel.GetCurSel();
			m_ctrlModel.SetItemHeight(nSel, 20);
		}
    }

   	str=cff.GetFileName();	
	if(cff.IsDirectory() && str.Find(_T("."))<0) 
	{
		m_ctrlModel.AddString(GetRecipModel(str));						// ComboBox에 넣는다.
		nSel = m_ctrlModel.GetCurSel();
		m_ctrlModel.SetItemHeight(nSel, 20);
	}
    
    cff.Close();
}

CString CDlgModelManager::GetRecipModel(CString str)
{
	CString strT, strModel;
	long i;

	m_editPriorityNum = _T("0");
	strT = str;
	for(i=0; i<MODEL_MAX_NUM; i++)
	{
		strModel.Format(_T("%s"), m_pMainFrm->m_Setting.m_ModelName[i]);
		if (strModel==str)
		{
			strT.Format(_T("[%02d]%s"), m_pMainFrm->m_Setting.m_OrderNum[i], str);
			m_editPriorityNum.Format(_T("%d"), m_pMainFrm->m_Setting.m_OrderNum[i]);
			break;
		}
	}

	return strT;
}

void CDlgModelManager::OnButtonSelect() 
{
	BOOL bRet;
	CString str, strT, strL, strModelName;
	int index;

	index=m_ctrlModel.GetCurSel();
	if (index<0)
		return;

	m_ctrlModel.GetText(index, strT);
    index = strT.Find(_T("]"))+1;
	strModelName = strT.Mid(index, strT.GetLength()-index);

	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	    strL = _T(" 모델이 비정상으로 해당 모델을 제거하시겠습니까 ? ");
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
		strL = _T(" This model was damaged. Do you want to delete it ? ");
	else
		strL = _T(" 此MODEL名为非正常化，是否删除该MODEL名 ？ ");

	bRet = LoadModelData(strModelName);
	if (bRet)
	{
        _tcscpy_s(m_pMainFrm->m_Setting.m_strModelName, strModelName);
		UpdateModelName();
	}
	else
	{
		//모델데이타가 불량이면 이전 모델데이타로 원상복구한다.
		if (MessageBox(strL, NULL, MB_YESNO)==IDYES)
		{
			DeleteModelData(strModelName);
			m_ctrlModel.ResetContent();
			FillListBoxWithFileName();
		}

		str = m_pMainFrm->m_Setting.m_strModelName;
		LoadModelData(str);

	    index=m_ctrlModel.FindString(0, str);
		if(index!=LB_ERR)
			m_ctrlModel.SetCurSel(index);
	}	
}

void CDlgModelManager::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	MakeNewModel();
	OnButtonSave();
	
	m_ctrlModel.ResetContent();
	FillListBoxWithFileName();
	
	UpdateModelName();
}

void CDlgModelManager::OnButtonSave() 
{
	CString str;
	str = m_pMainFrm->m_Setting.m_strModelName;

	SaveModelData(str);
	m_pMainFrm->SaveSysValueToFile();
}

void CDlgModelManager::OnButtonSaveas() 
{
	// TODO: Add your control notification handler code here
	int returnValue;
	CDlgModelName dlg;	
	CString str, strT, strL;
	CString FullPath;
	
	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	{
	    strT = _T(" 부적당한 모델명입니다. 다시 모델명을 입력하세요. ");
		strL = _T(" 모델명이 존재합니다. 덮어쓰기하시겠습니까 ? ");
	}
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
	{
		strT = _T(" The model is inadequate ! Enter model name again .");
		strL = _T(" There are the same model name. Do you want to overwrite it ? ");
	}
	else
	{
		strT = _T(" 这是不正确的MODEL名，请重新输入 ");
		strL = _T(" MODEL名已存在，是否要覆盖？ ");
	}

	dlg.m_sTitle.Format(_T(" Enter model name for saveas "));
	do
	{
		if(dlg.DoModal()!=IDOK) return;
	
		returnValue=CheckModelName(dlg.m_sModelName);
		if(returnValue==1)
			MessageBox(strT, NULL, MB_ICONEXCLAMATION&MB_ICONWARNING);

//		else if(returnValue==2)
//			MessageBox("Already Exist!!! Enter the Model Name Again.", NULL, MB_ICONEXCLAMATION&MB_ICONWARNING);
		else if(returnValue==2)
		{
			if(MessageBox(strL, NULL, MB_YESNO)==IDYES)
			{
				DeleteModelData(dlg.m_sModelName);
				returnValue=0;
			}
		}
	}while(returnValue);

	//새로운 모델의 폴더 생성한다. 
	str = dlg.m_sModelName;
	FullPath.Format(_T("%s%s"),MODEL_FILE_PATH, str);
	CreateDirectory(FullPath,NULL);

	//모델이름을 바꿔서 다시 저장한다.
	m_pMainFrm->SaveSysValueToFile();

	SaveModelData(str);	
	UpdateModelName();

	m_ctrlModel.ResetContent();
	FillListBoxWithFileName();
}

void CDlgModelManager::OnButtonRename() 
{
	int returnValue;
	CDlgModelName dlg;	
	CString SrcModelName, strT, strL;
	CString oldName, newName;

	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	{
	    strT = _T(" 부적당한 모델명입니다. 다시 모델명을 입력하세요. ");
		strL = _T(" 모델명이 존재합니다. 다시 모델명을 입력하세요. ");
	}
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
	{
		strT = _T(" The model is inadequate ! Enter model name again .");
		strL = _T(" There are the same model name. Enter model name again . ");
	}
	else
	{
		strT = _T(" 这是不正确的MODEL名，请重新输入 ");
		strL = _T(" MODEL名已存在，请重新输入 ");
	}

	dlg.m_sTitle.Format(_T(" Input new model name "));

	do 
	{
		if(dlg.DoModal()!=IDOK) return;
	
		returnValue=CheckModelName(dlg.m_sModelName);
		if(returnValue==1)
			MessageBox(strT, NULL, MB_ICONEXCLAMATION&MB_ICONWARNING);

		else if(returnValue==2)
			MessageBox(strL, NULL, MB_ICONEXCLAMATION&MB_ICONWARNING);
	} while(returnValue);
	
	oldName.Format(_T("%s%s"), MODEL_FILE_PATH, m_pMainFrm->m_Setting.m_strModelName);
	newName.Format(_T("%s%s"), MODEL_FILE_PATH, dlg.m_sModelName);
	CFile::Rename(oldName, newName);

	wsprintf(m_pMainFrm->m_Setting.m_strModelName, _T("%s"), (char*)LPCTSTR(dlg.m_sModelName));

	UpdateModelName();

	m_ctrlModel.ResetContent();
	FillListBoxWithFileName();
}

void CDlgModelManager::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	CString strL, strT, sModelName;
	int index;

	index=m_ctrlModel.GetCurSel();
	if (index<0) return;

	m_ctrlModel.GetText(index, strT);
    index = strT.Find(_T("]"))+1;
	sModelName = strT.Mid(index, strT.GetLength()-index);

	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	{
	    strT = _T(" 모델명이 사용중에 있습니다. ");
		strL = _T(" 모델을 제거하시겠습니까 ?  ");
	}
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
	{
		strT = _T(" The model is being used. ");
		strL = _T(" Are you sure you want to remove the model . ");
	}
	else
	{
		strT = _T(" 这是不正确的MODEL名 ");
		strL = _T(" 是否删除MODEL名？ ");
	}

	if (sModelName.Compare((TCHAR *)m_pMainFrm->m_Setting.m_strModelName)==0)
	{
		MessageBox(strT, NULL, MB_ICONEXCLAMATION&MB_ICONWARNING);
		return;
	}
	
	if (MessageBox(strL, NULL, MB_YESNO)==IDNO)
		return;

	DeleteModelData(sModelName);

	m_ctrlModel.ResetContent();
	FillListBoxWithFileName();
}

//신규 모델 생성 
void CDlgModelManager::MakeNewModel() 
{
	// TODO: Add your control notification handler code here
	int returnValue;
	CString strT, strL;
	CDlgModelName dlg;	

	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	{
	    strT = _T(" 부적당한 모델명입니다. 다시 모델명을 입력하세요. ");
		strL = _T(" 모델명이 존재합니다. 다시 모델명을 입력하세요. ");
	}
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
	{
		strT = _T(" The model is inadequate ! Enter model name again .");
		strL = _T(" There are the same model name. Enter model name again . ");
	}
	else
	{
		strT = _T(" 这是不正确的MODEL名，请重新输入 ");
		strL = _T(" MODEL名已存在，请重新输入 ");
	}
	
	do
	{
		if (dlg.DoModal()!=IDOK) return;
	
		returnValue=CheckModelName(dlg.m_sModelName);
		if(returnValue==1)
			MessageBox(strT, NULL, MB_ICONEXCLAMATION&MB_ICONWARNING);

		else if(returnValue==2)
			MessageBox(strL, NULL, MB_ICONEXCLAMATION&MB_ICONWARNING);

	}while(returnValue);

	MakeNewModelData(dlg.m_sModelName);
}

BOOL CDlgModelManager::LoadModelData(CString strFileName)
{
	long i;
	BOOL retValue, bRet = FALSE;
	CString filename;
	CFile f;

	filename.Format(_T("%s%s.bmp"), MODEL_IMAGE_PATH, strFileName);
	retValue = m_ImageBMP.Load(filename);

    GetRecipModel(strFileName);

	filename.Format(_T("%s%s\\Etc.dat"), MODEL_FILE_PATH, strFileName);
	if (f.Open(filename, CFile::modeRead)) 
	{
		f.Read(&m_pMainFrm->m_Etc, sizeof(ETC_DATA));
		f.Close();
	}

	for(i=0; i<VIEW_MAX_NUM; i++)
	{
		filename.Format(_T("%s%s\\Cam%d.dat"), MODEL_FILE_PATH, strFileName, i);
		if (NAppDll_LoadModelData(i, filename))
			bRet = TRUE;

		NAppDll_SetZoomScale(i, m_pMainFrm->m_Setting.m_fCamScale[i]);
	}

	NAppDll_InvalidateView(m_pMainFrm->m_nSelectCAM);

	UpdateData(false);
	Invalidate(false);

	return bRet;
}

BOOL CDlgModelManager::SaveModelData(CString strFileName)
{
	long i;
	BOOL bRet = FALSE;
	CFile f;
	CString filename;

	/*
	filename.Format("%s%s\\Etc.dat", MODEL_FILE_PATH, strFileName);
	if (f.Open(filename, CFile::modeCreate|CFile::modeWrite)) 
	{
		f.Write(&m_pMainFrm->m_Etc, sizeof(ETC_DATA));
		f.Close();
	}
	*/

	for(i=0; i<VIEW_MAX_NUM; i++)
	{
		filename.Format(_T("%s%s\\Cam%d.dat"), MODEL_FILE_PATH, strFileName, i);
		if (NAppDll_SaveModelData(i, filename))
			bRet = TRUE;
    }

	return bRet;
}

void CDlgModelManager::MakeNewModelData(CString FileName)
{
	long i;
	CString FullPath, filename;

	FullPath.Format(_T("%s%s"),MODEL_FILE_PATH, FileName);
	CreateDirectory(FullPath, NULL);
	
	m_pMainFrm->m_Etc.m_nTotalInspCount = 0;
	m_pMainFrm->m_Etc.m_nTotalGoodCount = 0;
	m_pMainFrm->m_Etc.m_nTotalBadCount = 0;
	m_pMainFrm->m_Etc.m_nUpBadCount = 0;
	m_pMainFrm->m_Etc.m_nDnBadCount = 0;
	m_pMainFrm->m_Etc.m_nUpFBadCount = 0;	
	m_pMainFrm->m_Etc.m_nUpFBadCount = 0;
	m_pMainFrm->m_Etc.m_nDnRBadCount = 0;	
	m_pMainFrm->m_Etc.m_nDnRBadCount = 0;


	wsprintf(m_pMainFrm->m_Setting.m_strModelName, _T("%s"),FileName);
	for(i=0; i<VIEW_MAX_NUM; i++)
	{
		NAppDll_CreateModelData(i, FullPath);
	}

//	filename.Format("%s%s.bmp", MODEL_IMAGE_PATH, FileName);
//	m_ImageBMP.Load(filename);
	Invalidate(false);
}

void CDlgModelManager::DeleteModelData(CString FileName)
{
	long i;
	CString dirname, fullPath,  filepath, filename;
	CFileFind cff;
	CString str1, str2;

	fullPath.Format(_T("%s%s\\*.*"),MODEL_FILE_PATH, FileName);
	dirname.Format(_T("%s%s"),MODEL_FILE_PATH, FileName);

	RemoveDir(dirname);

	for(i=0; i<MODEL_MAX_NUM; i++)
	{
		if (!_tcscmp(FileName, m_pMainFrm->m_Setting.m_ModelName[i]))
		{
			m_pMainFrm->m_Setting.m_OrderNum[i] = 0;
			_tcscpy_s(m_pMainFrm->m_Setting.m_ModelName[i], _T(""));
		}
	}

	m_ctrlModel.ResetContent();
	FillListBoxWithFileName();
}


void CDlgModelManager::OnDblclkListModel() 
{
	// TODO: Add your control notification handler code here
	OnButtonSelect();
}

//신규모델이름 체크
// return 0: 정상
//		  1: 이름에 이상한 문자가 섞여있어서 모델이름으로 적절치않음
//		  2: 같은이름의 모델이 이미 있음 
int CDlgModelManager::CheckModelName(CString sModelName)
{
	CFileFind cff;
    CString str;
	int returnValue=1;
	int N;

	sModelName.TrimLeft();

	N=sModelName.GetLength();
	if(N==0) goto done;

	/////////////////////////// 읽을 파일의 path를 지정한다.>>>>>>
	TCHAR FileName[80];
	_stprintf_s(FileName, _T("%s%s.*"), MODEL_FILE_PATH, sModelName);
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	if( cff.FindFile(FileName,0)) returnValue = 2; // FindFile이 함수를 실행한 후에야 										   
												   // FindNextFile 함수등을 쓸수 있다.	
	else returnValue=0;
    cff.Close();

done:
	return returnValue;
}

void CDlgModelManager::UpdateModelName()
{
	CString filename;

	m_sModelName.Format(_T("%s"),m_pMainFrm->m_Setting.m_strModelName);
	filename.Format(_T("%s%s.bmp"), MODEL_IMAGE_PATH, m_pMainFrm->m_Setting.m_strModelName);
    m_ImageBMP.Load(filename);

	UpdateData(false);
	Invalidate(false);
}

void CDlgModelManager::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	CString filename;

    if (bShow)
	{
		m_btnAutoRecip.SetCheck(m_pMainFrm->m_Setting.m_bRecipeOption);
		filename.Format(_T("%s%s.bmp"), MODEL_IMAGE_PATH, m_pMainFrm->m_Setting.m_strModelName);
	    m_ImageBMP.Load(filename);

        UpdateData(false);
		Invalidate(false);
	}
}

void CDlgModelManager::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcRect;

	CDC *pDC = m_staticFrame.GetDC();
    m_staticFrame.GetWindowRect(&rcRect);

	//{BEGIN : 화면에 영상그리기 >>>>>>>>>>>>>>>>>>>>>>>>
	CRect rc(0, 0, rcRect.Width(), rcRect.Height());
	m_ImageBMP.Draw(pDC->m_hDC, &rc, &rc);
	//}END : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	m_staticFrame.ReleaseDC(pDC);
}

void CDlgModelManager::OnBnClickedButtonApply()
{
	BOOL bFlag;
	long i, nIndex, nNum;
	CString strModelName, str, strT;

	UpdateData(true);

	nIndex=m_ctrlModel.GetCurSel();
	if (nIndex<0) 
		return;
		
	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	    strT = _T(" 변경된 데이터를 저장하시겠습니까 ? ");
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
		strT = _T(" Do you want apply the changed data ? ");
	else
		strT = _T(" 要保存变更的参数吗");

	if (MessageBox(strT, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		m_ctrlModel.GetText(nIndex, strT);
	    nIndex = strT.Find(_T("]"))+1;
	    strModelName = strT.Mid(nIndex, strT.GetLength()-nIndex);

		bFlag = FALSE;
		nIndex = 0;
		nNum = _ttoi(m_editPriorityNum);
		if (nNum>0 && nNum<20)
		{
			for(i=0; i<MODEL_MAX_NUM; i++)
			{
				str.Format(_T("%s"), m_pMainFrm->m_Setting.m_ModelName[i]);
				if (!bFlag && str=="") 
				{
					bFlag = TRUE;
					nIndex = i;
				}

				if (str==strModelName)
				{
					nIndex = i;
					break;
				}
			}

			for(i=0; i<MODEL_MAX_NUM; i++)
			{
				if (m_pMainFrm->m_Setting.m_OrderNum[i] == nNum)
					m_pMainFrm->m_Setting.m_OrderNum[i] = 0;
			}

			_tcscpy_s(m_pMainFrm->m_Setting.m_ModelName[nIndex], strModelName);
			m_pMainFrm->m_Setting.m_OrderNum[nIndex] = nNum;

			m_ctrlModel.ResetContent();
			FillListBoxWithFileName();

			m_pMainFrm->SaveSysValueToFile();
		}
	}

	UpdateData(false);
}

void CDlgModelManager::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void CDlgModelManager::OnBnClickedCheckAutoRecip()
{
	m_pMainFrm->m_Setting.m_bRecipeOption = m_btnAutoRecip.GetCheck();
}
