/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "pythoneditor.h"
#include "pythoneditorconstants.h"
#include "pythoneditorplugin.h"
#include "tools/pythonindenter.h"
#include "tools/pythonhighlighter.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>

#include <texteditor/texteditoractionhandler.h>
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/indenter.h>
#include <texteditor/autocompleter.h>

#include <utils/qtcassert.h>

#include <QFileInfo>

using namespace TextEditor;

namespace PythonEditor {
namespace Internal {

//////////////////////////////////////////////////////////////////
//
//  PythonEditor
//
//////////////////////////////////////////////////////////////////

PythonEditor::PythonEditor()
{
    addContext(Constants::C_PYTHONEDITOR_ID);
    setDuplicateSupported(true);
    setCommentStyle(Utils::CommentDefinition::HashStyle);
    setEditorCreator([]() { return new PythonEditor; });
    setWidgetCreator([]() { return new PythonEditorWidget; });

    setDocumentCreator([]() -> BaseTextDocument * {
        auto doc = new BaseTextDocument(Constants::C_PYTHONEDITOR_ID);
        doc->setIndenter(new PythonIndenter);
        new PythonHighlighter(doc);
        return doc;
    });
}

bool PythonEditor::open(QString *errorString,
                        const QString &fileName,
                        const QString &realFileName)
{
    Core::MimeType mimeType = Core::MimeDatabase::findByFile(QFileInfo(fileName));
    textDocument()->setMimeType(mimeType.type());
    return BaseTextEditor::open(errorString, fileName, realFileName);
}


//////////////////////////////////////////////////////////////////
//
//  PythonEditorWidget
//
//////////////////////////////////////////////////////////////////

PythonEditorWidget::PythonEditorWidget()
{
    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);
}

BaseTextEditor *PythonEditorWidget::createEditor()
{
    QTC_ASSERT("should not happen anymore" && false, return 0);
}


//////////////////////////////////////////////////////////////////
//
//  PythonEditorFactory
//
//////////////////////////////////////////////////////////////////

PythonEditorFactory::PythonEditorFactory()
{
    setId(Constants::C_PYTHONEDITOR_ID);
    setDisplayName(tr(Constants::C_EDITOR_DISPLAY_NAME));
    addMimeType(QLatin1String(Constants::C_PY_MIMETYPE));
    new TextEditorActionHandler(this, Constants::C_PYTHONEDITOR_ID,
                              TextEditorActionHandler::Format
                              | TextEditorActionHandler::UnCommentSelection
                              | TextEditorActionHandler::UnCollapseAll);
}

Core::IEditor *PythonEditorFactory::createEditor()
{
    return new PythonEditor;
}

} // namespace Internal
} // namespace PythonEditor
