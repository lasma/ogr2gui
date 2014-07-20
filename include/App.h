/*****************************************************************************
 * ogr2gui is an application used to convert and manipulate geospatial
 * data. It is based on the "OGR Simple Feature Library" from the
 * "Geospatial Data Abstraction Library" <http://gdal.org>.
 *
 * Copyright (c) 2009 Inventis <mailto:developpement@inventis.ca>
 * Copyright (c) 2014 Faculty of Computer Science,
 * University of Applied Sciences Rapperswil (HSR),
 * 8640 Rapperswil, Switzerland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

/**
 *	\mainpage ogr2gui
 *	\section desc_sec Description
 *
 *	ogr2gui is an application used to convert and manipulate geospatial data.
 *  It is based on ogr2ogr, a command line utility from the "Geospatial Data
 *  Abstraction Library" (gdal.org). Through its graphical user interface,
 *  ogr2gui gives all the power of ogr2ogr without worrying about its complex
 *  syntax. It brings speed, efficiency and simplicity to its users. ogr2gui
 *  is a free, open source project released under GPL License. Everyone in
 *  the geospatial community is invited to join. If you want to get involved
 *  in the project development, please contact us at dev@ogr2gui.ca
 *
 *	\section supp_sec Support
 *	<a href="mailto:dev@ogr2gui.ca">dev@ogr2gui.ca</a>
 */

/**
 *	\file App.h
 *	\brief Qt Application
 *	\author Olivier Pilotte [ Inventis ], Mohamed Hedi Lassoued [ Inventis ], David Tran [ HSR ]
 *	\version 0.7
 *	\date 13/06/14
 */

#ifndef APP
#define APP

#include "Ogr.h"
#include "Inf.h"
#include "wfsConnect.h"

QT_BEGIN_NAMESPACE

class App : public QMainWindow
{
    Q_OBJECT

private:
    Ogr *ogr;

    Inf *inf;

    WFSConnect *wfs;

    // ogr2ogr parameters
    QString parameters;

    // file formats
    const static int formatsCount = 47;
    QString **formats;

    // output formats
    const static int formatsOutput = 28;

    // database formats
    const static int databasesCount = 4;
    QString **databases;

    // output databases
    const static int databasesOutput = 2;

    // web service formats
    const static int webservicesCount = 1;
    QString **webservices;

    // target projections
    QList<QPair<QString, QString> > projectionsList;

    // qt
    QMenuBar *theMenu;
    QMenu *fileMenu;
    QMenu *helpMenu;

    QMenu *mnuLanguage;
    QAction *mnuEnglish;
    QAction *mnuFrench;
    QAction *mnuGerman;
    QAction *mnuItalian;
    QAction *mnuSpanish;
    QAction *mnuChinese;
    QAction *mnuRussian;
    QAction *mnuArabic;

    QAction *mnuExit;
    QAction *mnuOgrHelp;
    QAction *mnuGuiHelp;
    QAction *mnuOgrinfo;
    QAction *mnuHsrAbout;
    QAction *mnuAbout;

    QStatusBar *statusbar;

    QProgressBar *theProgress;

    QWidget *thePanel;
    QVBoxLayout *theLayout;
    QGroupBox *grpSource;
    QGridLayout *lytSource;

    QLabel *lblSourceFormat;
    QComboBox *cmbSourceFormat;

    QHBoxLayout *lytSourceInput;
    QRadioButton *radSourceFile;
    QRadioButton *radSourceFolder;
    QRadioButton *radSourceDatabase;
    QRadioButton *radSourceWebservice;

    QLabel *lblSourceName;
    QHBoxLayout *lytSourceName;
    QLineEdit *txtSourceName;
    QPushButton *btnSourceName;

    QLabel *lblSourceProj;
    QLineEdit *txtSourceProj;

    QLabel *lblSourceQuery;
    QLineEdit *txtSourceQuery;

    QGroupBox *grpTarget;
    QGridLayout *lytTarget;

    QHBoxLayout *lytTargetOutput;
    QButtonGroup *grpTargetOutput;
    QRadioButton *radTargetFile;
    QRadioButton *radTargetFolder;
    QRadioButton *radTargetDatabase;

    QLabel *lblTargetFormat;
    QComboBox *cmbTargetFormat;

    QLabel *lblTargetName;
    QHBoxLayout *lytTargetName;
    QLineEdit *txtTargetName;
    QPushButton *btnTargetName;

    QLabel *lblTargetProj;
    QHBoxLayout *lytTargetProj;
    QLineEdit *txtTargetProj;
    QComboBox *cmbTargetProj;

    QHBoxLayout *lytTargetOptions;
    QButtonGroup *grpTargetOptions;
    QCheckBox *radTargetAppend;
    QCheckBox *radTargetOverwrite;
    QCheckBox *radTargetUpdate;

    QGroupBox *grpOptions;
    QGridLayout *lytOptions;

    QTextEdit *txtOutput;
    QTextEdit *txtInput;

    QHBoxLayout *lytExecute;
    QPushButton *btnExecute;
    QPushButton *btnExit;

    /**
         *	\fn void InitData( void );
         *	\brief Inits data
         */
    void InitData( void );

    /**
         *	\fn void void InitProjections( void );
         *	\brief Inits projections
         */
    void InitProjections( void );

    /**
         *	\fn void InitInterface( void );
         *	\brief Inits Interface
         */
    void InitInterface( void );

    /**
         *	\fn void InitMenu( void );
         *	\brief Inits Menu
         */
    void InitMenu( void );

    /**
         *	\fn void InitLayout( void );
         *	\brief Inits Layout
         */
    void InitLayout( void );

    /**
         *	\fn void InitSlots( void );
         *	\brief Inits Slots
         */
    void InitSlots( void );

    /**
         *	\fn void TranslateInterface( void );
         *	\brief Translates Interface
         */
    void TranslateInterface( void );

    /**
         *	\fn void UpdateParameters( void );
         *	\brief Updates parameters
         */
    void UpdateParameters( void );

    /**
         * \brief QString currentParameters(void);
         * \return QString with parameters
         */
    QString currentParameters(void);


private slots :
    void evtMnuOgrHelp( void );
    void evtMnuGuiHelp( void );
    void evtMnuOgrinfo(void);
    void evtMnuHsrAbout( void );
    void evtMnuOgrAbout( void );

    void evtRadSourceFile( void );
    void evtRadSourceFolder( void );
    void evtRadSourceDatabase( void );
    void evtRadSourceWebservice( void );

    void evtCmbSourceFormat( void );
    void evtTxtSourceName( void );
    void evtBtnSourceName( void );

    void evtRadTargetFile( void );
    void evtRadTargetFolder( void );
    void evtRadTargetDatabase( void );

    void evtCmbTargetFormat( void );
    void evtTxtTargetName( void );
    void evtBtnTargetName( void );
    void evtTxtTargetProj( void );

    void evtUpdateParameters( void );

    void evtBtnExecute( void );
    void evtBtnQuit( void );

public:

    /**
         *	\fn App( QWidget * = 0 );
         *	\brief Constructor
         */
    App( QWidget * = 0 );

    /**
         *	\fn ~App
         *	\brief Destructor
         */
    ~App( void );
};

QT_END_NAMESPACE

#endif
