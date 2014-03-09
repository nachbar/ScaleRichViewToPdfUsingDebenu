// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DebenuPDFLibraryDLL1013.h"
#include "DebenuPDFLicenseKey.h"
#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RVScroll"
#pragma link "SclRView"
#pragma resource "*.dfm"
TForm1 *Form1;

const int    RTFPRINTINGDOTSPERINCH = 96;
const double RTFPAGEHEIGHTININCHES  = 11;
const double RTFPAGEWIDTHININCHES   = 8.5;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) :
	TForm(Owner)
{
}
// ---------------------------------------------------------------------------
typedef DebenuPDFLibraryDLL1013 DebenuPDFLibrary;

DebenuPDFLibrary* GetGlobalDebenu(
	bool fClearDocuments = false,
	bool fReleaseLibrary = false) // default false, false
{
	static DebenuPDFLibrary* gDebenuLibrary         = NULL;
	static int               gDebenuLibraryUnlocked = 0;
	if (fReleaseLibrary)
	{
		if (gDebenuLibrary)
		{
			delete(gDebenuLibrary);
			gDebenuLibrary = NULL;
		}
		return NULL;
	}
	if (gDebenuLibrary == NULL)
	{
		gDebenuLibrary = new DebenuPDFLibraryDLL1013(L"DebenuPDFLibraryDLL1013.dll");
		bool fLoaded   = gDebenuLibrary->LibraryLoaded();
		if (!fLoaded)
		{
			delete gDebenuLibrary;
			gDebenuLibrary = NULL;
			throw Exception("Unable to load Debenu library");
		}
	}
	if (!gDebenuLibraryUnlocked)
	{
		// #define DEBENULICENSEKEY L"xxxabc123" // include your real (or trial) license key here or elsewhere
		gDebenuLibrary->UnlockKey(DEBENULICENSEKEY);

		gDebenuLibraryUnlocked = gDebenuLibrary->Unlocked();
		if (!gDebenuLibraryUnlocked)
		{
			delete gDebenuLibrary; // so we will reload next time
			gDebenuLibrary = NULL;
			throw Exception("Unable to unlock Debenu library");
		}
		gDebenuLibrary->SetOrigin(1); // top left -- set when library first loaded and unlocked
	}
	if (fClearDocuments)
	{
		while (gDebenuLibrary->DocumentCount() > 1)
		{
			gDebenuLibrary->RemoveDocument(gDebenuLibrary->GetDocumentID(1));
		}
		// Remove the last document.  It will be replaced with a new, blank document, so that DocumentCount() will still be 1
		gDebenuLibrary->RemoveDocument(gDebenuLibrary->GetDocumentID(1));
		gDebenuLibrary->SetOrigin(1); // top left -- set when documents cleared
	}
	return gDebenuLibrary;
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	int               outputDocID = 0; // keeps track of Debenu PDF Document that is collecting the pages
	DebenuPDFLibrary* debenu      = GetGlobalDebenu(true); // clear documents

	TCanvas* canvas    = NULL;
	int      PageCount = SRichViewEdit1->PageCount;
	for (int i = 0; i < PageCount; i++)
	{
		HDC hdcNew = debenu->GetCanvasDC(
			RTFPRINTINGDOTSPERINCH * RTFPAGEWIDTHININCHES,
			RTFPRINTINGDOTSPERINCH * RTFPAGEHEIGHTININCHES);
		canvas         = new TCanvas;
		canvas->Handle = hdcNew;

		SRichViewEdit1->DrawPage(
			i + 1, // Page Number
			int(RTFPRINTINGDOTSPERINCH * RTFPAGEWIDTHININCHES), // Page Width
			int(RTFPRINTINGDOTSPERINCH * RTFPAGEHEIGHTININCHES), // Page Height
			0, // Left margin - if margins already defined for TSRichView, this is additional
			0,      // Top margin - if margins already defined for TSRichView, this is additional
			canvas, // TCanvas
			true,   // Print page number on page
			false,  // Clip Margins
			true);  // Printing flag

		int loadResult = debenu->LoadFromCanvasDC(RTFPRINTINGDOTSPERINCH, 0);
		// creates a new document - 50 DPI
		delete canvas;
		canvas = NULL;
		if (loadResult)
		{
			if (outputDocID == 0)
			{
				outputDocID = debenu->SelectedDocument();
			}
			else
			{
				int newLoadedDocument = debenu->SelectedDocument();
				debenu->SelectDocument(outputDocID);
				debenu->MergeDocument(newLoadedDocument); // this deletes the second document
			}
		}

	}
	debenu->SelectDocument(outputDocID);
	debenu->SaveToFile("c:\\testtrichview.pdf");
}
// ---------------------------------------------------------------------------
