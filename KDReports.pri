#include($$PWD/KDReports-master/kdchart.pri)
#include($$PWD/KDReports-master/variables.pri)

FORMS += $$PWD/KDReports-master/src/KDReports/previewdialogbase.ui \
    $$PWD/KDReports-master/src/KDReports/tablebreakingdialogbase.ui


RESOURCES += $$PWD/KDReports-master/pics/pics.qrc

PUBLICHEADERS = $$PWD/KDReports-master/src/KDReports/KDReportsReport.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsChartElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsTextElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsHtmlElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsTableElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsAutoTableElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsAbstractTableElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsImageElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsHLineElement.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsPreviewDialog.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsPreviewWidget.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsHeader.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsCell.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsGlobal.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlElementHandler.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlElementHandlerV2.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsVariableType.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsUnit.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsTableBreakingSettingsDialog.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsMainTable.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlHelper.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsErrorDetails.h

PRIVATEHEADERS = $$PWD/KDReports-master/src/KDReports/KDReportsHeader_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsReport_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlParser_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsErrorDetails_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsAbstractReportLayout_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsTextDocReportLayout_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsSpreadsheetReportLayout_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsHLineTextObject_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsChartTextObject_p.h \
    $$PWD/KDReports-master/src/KDReports/KDReportsTableLayout_p.h

HEADERS += $$PUBLICHEADERS \
    $$PRIVATEHEADERS \

SOURCES += $$PWD/KDReports-master/src/KDReports/KDReportsReport.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsChartElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTextElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsHtmlElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTableElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsAutoTableElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsAbstractTableElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsImageElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsReportBuilder.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsPreviewDialog.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsPreviewWidget.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsLayoutHelper.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsHeader.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTextDocument.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTextDocumentData.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsCell.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlParser.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTableBreakingSettingsDialog.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlElementHandler.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsErrorDetails.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTableBreakingLogic.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsFontScaler.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsHLineTextObject.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsHLineElement.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsChartTextObject.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsMainTable.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsAbstractReportLayout.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTextDocReportLayout.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsSpreadsheetReportLayout.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsTableLayout.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlElementHandlerV2.cpp \
    $$PWD/KDReports-master/src/KDReports/KDReportsXmlHelper.cpp
