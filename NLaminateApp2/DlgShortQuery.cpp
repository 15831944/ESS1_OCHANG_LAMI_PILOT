// DlgShortQuery.cpp : implementation file
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgShortQuery.h"
#include "afxdialogex.h"


// CDlgShortQuery dialog

IMPLEMENT_DYNAMIC(CDlgShortQuery, CDialogEx)

CDlgShortQuery::CDlgShortQuery(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShortQuery::IDD, pParent)
{

}

CDlgShortQuery::~CDlgShortQuery()
{
}

void CDlgShortQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgShortQuery, CDialogEx)
END_MESSAGE_MAP()


// CDlgShortQuery message handlers


BOOL CDlgShortQuery::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
