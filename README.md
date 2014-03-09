###This project shows how to use the Debenu Quick PDF Library to create a PDF from a TSRichViewEdit in C++ Builder.  The Scale Rich View is part of the TRichView suite.

The Debenu Quick PDF Library is available in several versions.  Here, I am using the DLL version.

I am not including any of the Debenu (or TRichView) files in this project, because they are proprietary software, and I am not affiliated with either company.  This project is provided as sample code, with NO WARRANTY.  I assume that you will have to inspect it and modify it for your purposes, but this will get you started.

To start, purchase (or download the trial) and install the Debenu Quick PDF Library.  Go to C:\Program Files (x86)\Debenu\PDF Library\DLL\Import\CPlusPlus and copy the DebenuPDFLibraryDLL1013.cpp and DebenuPDFLibraryDLL10113.h (the versions may be different) into this project.

Then, apply the patchDebenuCPlusPlus.patch patch to the Debenu .cpp and .h files:

To view the stats of the patch:
git apply --stat patchDebenuCPlusPlus.patch

To try out the patch:
git apply --check patchDebenuCPlusPlus.patch

To finally apply and sign off on the patch:
git am --signoff < patchDebenuCPlusPlus.patch

Then, you may be able to load the Project into C++ Builder XE5 (you might need to modify the project file if you have different components installed.)  Another option is to create a new project and add the Unit1.cpp and the (patched) DebenuPDFLibraryDLL1013.cpp file.  The Unit1 has some TRichView components, so you will need to make sure that your project has access to the files to support that (in my configuration, I include the RVPkgDXE5.lib and other .lib's (which I created when I installed TRichView for both Delphi and C++ Builder) in the project.

Then, build and run the application.

It will call SRichViewEdit1->DrawPage() to draw each page onto a Canvas supplied by the Debenu library, and then, at the end, save the .pdf file to a file.

If you have not copied the DebenuPDFLibraryDLL1013.dll file into the same folder as your executable, you will see "Unable to load Debenu library"

Again, this is intended as sample code, and will need to be modified to be useful.
