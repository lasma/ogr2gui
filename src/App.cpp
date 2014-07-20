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

/*!
 *	\file App.cpp
 *	\brief Qt Application
 *	\author Olivier Pilotte [ Inventis ], Mohamed Hedi Lassoued [ Inventis ], David Tran [ HSR ]
 *	\version 0.7
 *	\date 13/06/14
 */

#include "App.h"

App::App(QWidget *widget) : QMainWindow(widget)
{
    inf = new Inf(this);
    wfs = new WFSConnect(this);
    InitData();
    InitInterface();
    TranslateInterface();
    InitProjections();
    UpdateParameters();
    this->show();
}

App::~App( void )
{
    delete [] *formats;
    delete [] *databases;
    delete [] *webservices;
}

void App::InitData(void) {
    ogr = new Ogr();

    formats = new QString*[formatsCount];
    for(int i = 0; i < formatsCount; ++i) {
        formats[i] = new QString[2];
    }
    databases = new QString *[databasesCount];
    for(int i = 0; i < databasesCount; ++i) {
        databases[i] = new QString[2];
    }
    webservices = new QString*[webservicesCount];
    for(int i = 0; i < webservicesCount; ++i) {
        webservices[i] = new QString[2];
    }

#include "Dta.h"
}

void App::InitProjections( void )
{
    QString folder = tr("data");
    QString gcs = tr("gcs.csv");
    QString filename = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QDir::separator() + folder + QDir::separator() + gcs);
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setText("* No " + gcs + " file found in folder data");
        msg.exec();
        return;
    }
    QTextStream in(&file);
    QString line;
    QPair<QString, QString> pair;
    projectionsList << pair;
    cmbTargetProj->addItem(tr(""));
    in.readLine();
    while(!(line = in.readLine()).isNull()) {
        QStringList t = line.split(",");
        if(t.size() <= 1) {
            QMessageBox msg;
            msg.setText("* Wrong " + gcs + " file found in folder data");
            msg.exec();
            break;
        }
        if(!t.at(0).isNull())
            pair.first = t.at(0);
        if(!t.at(1).isNull())
            pair.second = t.at(1);
        projectionsList << pair;
        cmbTargetProj->addItem(t.at(0) + tr(" : ") + t.at(1));
    }
}

void App::InitInterface( void )
{
    thePanel = new QWidget();

    InitMenu();
    InitLayout();
    InitSlots();

    radSourceFile->setChecked(true);
    radTargetFile->setChecked(true);
    radTargetOverwrite->setChecked(true);
    btnExecute->setEnabled(false);

    this->setCentralWidget(thePanel);
}

void App::InitMenu( void )
{
    theMenu = new QMenuBar( this );
    {
        fileMenu = new QMenu( theMenu );
        {
            mnuOgrinfo = new QAction(this);
            mnuExit = new QAction( this );
            mnuExit->setShortcuts(QKeySequence::Quit);

            fileMenu->addAction(mnuOgrinfo);
            fileMenu->addSeparator();
            fileMenu->addAction( mnuExit );
        }

        helpMenu = new QMenu( theMenu );
        {
            mnuOgrHelp = new QAction( this );
            mnuOgrHelp->setShortcuts(QKeySequence::HelpContents);
            mnuGuiHelp = new QAction( this );
            mnuGuiHelp->setShortcuts(QKeySequence::WhatsThis);
            mnuHsrAbout = new QAction( this );
            mnuAbout = new QAction( this );

            helpMenu->addAction( mnuOgrHelp );
            helpMenu->addAction( mnuGuiHelp );
            helpMenu->addAction(mnuHsrAbout);
            helpMenu->addSeparator();
            helpMenu->addAction( mnuAbout );
        }
        theMenu->addAction( fileMenu->menuAction() );
        theMenu->addAction( helpMenu->menuAction() );
    }

    this->setMenuBar( theMenu );
}

void App::InitLayout( void )
{
    theLayout = new QVBoxLayout( thePanel );
    {
        theLayout->setMargin( 7 );
        theLayout->setSpacing( 7 );

        grpSource = new QGroupBox( thePanel );
        {
            grpSource->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

            lytSource = new QGridLayout();
            {
                lytSource->setMargin( 7 );
                lytSource->setSpacing( 7 );

                lytSourceInput = new QHBoxLayout();
                {
                    radSourceFile = new QRadioButton();
                    radSourceFolder = new QRadioButton();
                    radSourceDatabase = new QRadioButton();
                    radSourceWebservice = new QRadioButton();

                    lytSourceInput->addWidget( radSourceFile );
                    lytSourceInput->addWidget( radSourceFolder );
                    lytSourceInput->addWidget( radSourceDatabase );
                    lytSourceInput->addWidget( radSourceWebservice);
                }

                lytSource->addLayout( lytSourceInput, 0, 1 );

                lblSourceFormat = new QLabel();
                lblSourceFormat->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
                lblSourceFormat->setMinimumWidth( 70 );
                lblSourceFormat->setMaximumWidth( 70 );

                cmbSourceFormat = new QComboBox();

                lytSource->addWidget( lblSourceFormat, 1, 0 );
                lytSource->addWidget( cmbSourceFormat, 1, 1 );

                lblSourceName = new QLabel();
                lblSourceName->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
                lblSourceName->setMinimumWidth( 70 );
                lblSourceName->setMaximumWidth( 70 );

                lytSourceName = new QHBoxLayout();
                {
                    txtSourceName = new QLineEdit();

                    btnSourceName = new QPushButton();

                    lytSourceName->addWidget( txtSourceName );
                    lytSourceName->addWidget( btnSourceName );
                }

                lytSource->addWidget( lblSourceName, 2, 0 );
                lytSource->addLayout( lytSourceName, 2, 1 );

                lblSourceProj = new QLabel();
                lblSourceProj->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
                lblSourceProj->setMinimumWidth( 70 );
                lblSourceProj->setMaximumWidth( 70 );

                txtSourceProj = new QLineEdit();
                txtSourceProj->setReadOnly( true );

                lytSource->addWidget( lblSourceProj, 3, 0 );
                lytSource->addWidget( txtSourceProj, 3, 1 );

                lblSourceQuery = new QLabel();
                lblSourceQuery->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
                lblSourceQuery->setMinimumWidth( 70 );
                lblSourceQuery->setMaximumWidth( 70 );

                txtSourceQuery = new QLineEdit();

                lytSource->addWidget( lblSourceQuery );
                lytSource->addWidget( txtSourceQuery );
            }

            grpSource->setLayout( lytSource );
        }

        theLayout->addWidget( grpSource );

        grpTarget = new QGroupBox( thePanel );
        {
            grpTarget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

            lytTarget = new QGridLayout();
            {
                lytTargetOutput = new QHBoxLayout();
                {
                    grpTargetOutput = new QButtonGroup();
                    {
                        radTargetFile = new QRadioButton();
                        radTargetFolder = new QRadioButton();
                        radTargetDatabase = new QRadioButton();

                        grpTargetOutput->addButton( radTargetFile );
                        grpTargetOutput->addButton( radTargetFolder );
                        grpTargetOutput->addButton( radTargetDatabase );

                        lytTargetOutput->addWidget( radTargetFile );
                        lytTargetOutput->addWidget( radTargetFolder );
                        lytTargetOutput->addWidget( radTargetDatabase );
                    }
                }

                lytTarget->addLayout( lytTargetOutput, 0, 1 );


                lblTargetFormat = new QLabel();
                lblTargetFormat->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
                lblTargetFormat->setMinimumWidth( 70 );
                lblTargetFormat->setMaximumWidth( 70 );

                cmbTargetFormat = new QComboBox();

                lytTarget->addWidget( lblTargetFormat, 1, 0 );
                lytTarget->addWidget( cmbTargetFormat, 1, 1 );


                lblTargetName = new QLabel();
                lblTargetName->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
                lblTargetName->setMinimumWidth( 70 );
                lblTargetName->setMaximumWidth( 70 );

                lytTargetName = new QHBoxLayout();
                {
                    txtTargetName = new QLineEdit();
                    txtTargetName->setReadOnly(true);

                    btnTargetName = new QPushButton();

                    lytTargetName->addWidget( txtTargetName );
                    lytTargetName->addWidget( btnTargetName );
                }

                lytTarget->addWidget( lblTargetName, 2, 0 );
                lytTarget->addLayout( lytTargetName, 2, 1 );


                lblTargetProj = new QLabel();
                lblTargetProj->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
                lblTargetProj->setMinimumWidth( 70 );
                lblTargetProj->setMaximumWidth( 70 );

                lytTargetProj = new QHBoxLayout();
                {
                    txtTargetProj = new QLineEdit();
                    txtTargetProj->setMaxLength( 5 );
                    txtTargetProj->setMinimumWidth( 50 );
                    txtTargetProj->setMaximumWidth( 50 );
                    QValidator *validator = new QIntValidator(0, 99999, this);
                    txtTargetProj->setValidator(validator);

                    cmbTargetProj = new QComboBox();
                    cmbTargetProj->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

                    lytTargetProj->addWidget( txtTargetProj );
                    lytTargetProj->addWidget( cmbTargetProj );
                }

                lytTarget->addWidget( lblTargetProj, 3, 0 );
                lytTarget->addLayout( lytTargetProj, 3, 1 );

                lytTargetOptions = new QHBoxLayout();
                {
                    radTargetOverwrite = new QCheckBox();
                    radTargetAppend = new QCheckBox();
                    radTargetUpdate = new QCheckBox();

                    lytTargetOptions->addWidget( radTargetOverwrite );
                    lytTargetOptions->addWidget( radTargetAppend );
                    lytTargetOptions->addWidget( radTargetUpdate );
                }
                lytTarget->addLayout( lytTargetOptions, 4, 1 );
            }
            grpTarget->setLayout( lytTarget );
        }

        theLayout->addWidget( grpTarget );

        grpOptions = new QGroupBox( thePanel );
        {
            grpOptions->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

            lytOptions = new QGridLayout();
            {
                txtInput = new QTextEdit();
                txtInput->setMaximumHeight(80);
            }
            lytOptions->addWidget(txtInput, 1, 0);
            grpOptions->setLayout(lytOptions);
        }

        theLayout->addWidget(grpOptions);

        txtOutput = new QTextEdit();
        txtOutput->setReadOnly(true);

        lytExecute = new QHBoxLayout();
        {
            btnExecute = new QPushButton();
            btnExecute->setMinimumWidth( 200 );
            btnExecute->setEnabled( false );

            btnExit = new QPushButton();
            btnExit->setMinimumWidth( 200 );

            lytExecute->addWidget( btnExecute );
            lytExecute->addWidget( btnExit );
        }

        theLayout->addWidget( txtOutput );
        theLayout->addLayout( lytExecute );

        theProgress = new QProgressBar();
        theProgress->setValue(0);

        theLayout->addWidget( theProgress );
    }

    thePanel->setLayout( theLayout );
}

void App::InitSlots( void )
{
    QObject::connect( mnuExit, SIGNAL( triggered() ), this, SLOT( close( void ) ) );
    QObject::connect( mnuOgrHelp, SIGNAL( triggered() ), this, SLOT( evtMnuOgrHelp( void ) ) );
    QObject::connect( mnuGuiHelp, SIGNAL( triggered() ), this, SLOT( evtMnuGuiHelp( void ) ) );
    QObject::connect(mnuOgrinfo, SIGNAL(triggered()), this, SLOT(evtMnuOgrinfo(void)));
    QObject::connect( mnuAbout, SIGNAL( triggered() ), this, SLOT( evtMnuOgrAbout( void ) ) );
    QObject::connect(mnuHsrAbout, SIGNAL(triggered()), this, SLOT(evtMnuHsrAbout(void)));

    QObject::connect( radSourceFile, SIGNAL( toggled( bool ) ), this, SLOT( evtRadSourceFile( void ) ) );
    QObject::connect( radSourceFolder, SIGNAL( toggled( bool ) ), this, SLOT( evtRadSourceFolder( void ) ) );
    QObject::connect( radSourceDatabase, SIGNAL( toggled( bool ) ), this, SLOT( evtRadSourceDatabase( void ) ) );
    QObject::connect( radSourceWebservice, SIGNAL( toggled( bool ) ), this, SLOT( evtRadSourceWebservice( void ) ) );

    QObject::connect( cmbSourceFormat, SIGNAL( currentIndexChanged( int ) ), this, SLOT( evtCmbSourceFormat( void ) ) );
    QObject::connect( txtSourceName, SIGNAL( textChanged( QString ) ), this, SLOT( evtTxtSourceName( void ) ) );
    QObject::connect( btnSourceName, SIGNAL( clicked( void ) ), this, SLOT( evtBtnSourceName( void ) ) );
    QObject::connect( txtSourceQuery, SIGNAL( textChanged( QString ) ), this, SLOT( evtUpdateParameters( void ) ) );
    QObject::connect( txtInput, SIGNAL( textChanged() ), this, SLOT( evtUpdateParameters( void ) ) );

    QObject::connect( radTargetFile, SIGNAL( toggled( bool ) ), this, SLOT( evtRadTargetFile( void ) ) );
    QObject::connect( radTargetFolder, SIGNAL( toggled( bool ) ), this, SLOT( evtRadTargetFolder( void ) ) );
    QObject::connect( radTargetDatabase, SIGNAL( toggled( bool ) ), this, SLOT( evtRadTargetDatabase( void ) ) );

    QObject::connect( cmbTargetFormat, SIGNAL( currentIndexChanged( int ) ), this, SLOT( evtCmbTargetFormat( void ) ) );
    QObject::connect( txtTargetName, SIGNAL( textChanged( QString ) ), this, SLOT( evtTxtTargetName( void ) ) );
    QObject::connect( btnTargetName, SIGNAL( clicked() ), this, SLOT( evtBtnTargetName( void ) ) );
    QObject::connect( txtTargetProj, SIGNAL( textChanged( QString ) ), this, SLOT( evtTxtTargetProj( void ) ) );
    QObject::connect( cmbTargetProj, SIGNAL( currentIndexChanged( int ) ), this, SLOT( evtUpdateParameters( void ) ) );

    QObject::connect( radTargetOverwrite, SIGNAL( toggled( bool ) ), this, SLOT( evtUpdateParameters( void ) ) );
    QObject::connect( radTargetAppend, SIGNAL( toggled( bool ) ), this, SLOT( evtUpdateParameters( void ) ) );
    QObject::connect( radTargetUpdate, SIGNAL( toggled( bool ) ), this, SLOT( evtUpdateParameters( void ) ) );

    QObject::connect( btnExecute, SIGNAL( clicked( void ) ), this, SLOT( evtBtnExecute( void ) ) );
    QObject::connect( btnExit, SIGNAL( clicked( void ) ), this, SLOT( evtBtnQuit( void ) ) );

    QMetaObject::connectSlotsByName( this );
}

void App::TranslateInterface( void )
{
    this->setWindowTitle( tr( "OGR2GUI" ) );

    fileMenu->setTitle( tr( "File" ) );
    {
        mnuOgrinfo->setText(tr("Ogrinfo Documentation"));
        mnuExit->setText( tr( "Exit" ) );
    }

    helpMenu->setTitle( tr( "Help" ) );
    {
        mnuOgrHelp->setText( tr( "Command-Line Options" ) );
        mnuGuiHelp->setText( tr( "Documentation" ) );
        mnuHsrAbout->setText((tr("About HSR")));
        mnuAbout->setText( tr( "About OGR2GUI" ) );
    }

    grpSource->setTitle( tr( "Source" ) );
    {
        radSourceFile->setText( tr( "File" ) );
        radSourceFolder->setText( tr( "Folder" ) );
        radSourceDatabase->setText( tr( "Database" ) );
        radSourceWebservice->setText( tr( "Web Service" ) );

        lblSourceFormat->setText( tr( "Format" ) );

        lblSourceName->setText( tr( "Name" ) );
        btnSourceName->setText( tr( "Open" ) );

        lblSourceProj->setText( tr( "Projection" ) );

        lblSourceQuery->setText( tr( "Query" ) );
    }

    grpTarget->setTitle( tr( "Target" ) );
    {
        radTargetFile->setText( tr( "File" ) );
        radTargetFolder->setText( tr( "Folder" ) );
        radTargetDatabase->setText( tr( "Database" ) );

        lblTargetFormat->setText( tr( "Format" ) );

        lblTargetName->setText( tr( "Name" ) );
        btnTargetName->setText( tr( "Save" ) );

        lblTargetProj->setText( tr( "Projection" ) );

        radTargetOverwrite->setText( tr( "overwrite" ) );
        radTargetAppend->setText( tr( "append" ) );
        radTargetUpdate->setText( tr( "update" ) );
    }

    grpOptions->setTitle(tr("Options (optional)"));

    btnExecute->setText( tr( "Execute" ) );
    btnExit->setText( tr( "Exit" ) );
}

void App::UpdateParameters(void) {
    parameters = tr("ogr2ogr");
    parameters += currentParameters();
    if(radSourceWebservice->isChecked())
        parameters += tr(" ") + wfs->getSelectedLayers();
    if(!txtInput->toPlainText().isEmpty())
        parameters += tr(" ") + txtInput->toPlainText().simplified();
    txtOutput->setText(parameters);
    theProgress->setValue(0);
}

QString App::currentParameters(void) {
    QString parameters = tr(" -f ") + tr("\"") + cmbTargetFormat->currentText() + tr("\" ");
    parameters += tr("\"") + txtTargetName->text()+ tr("\" ");
    if(radSourceWebservice->isChecked() && !cmbSourceFormat->currentText().isEmpty())
        parameters += webservices[cmbSourceFormat->currentIndex()][1];
    parameters += tr("\"") + txtSourceName->text().trimmed() + tr("\"");
    if(!cmbTargetProj->currentText().isEmpty()) {
        parameters += tr(" ") + tr("-t_srs");
        parameters += tr(" EPSG:") + projectionsList.at(cmbTargetProj->currentIndex()).first;
    }
    if(!txtSourceQuery->text().isEmpty())
        parameters += tr(" -sql ") + tr("\"") + txtSourceQuery->text() + tr("\"");
    if(radTargetOverwrite->isChecked())
        parameters += tr(" -overwrite");
    if(radTargetAppend->isChecked())
        parameters += tr(" -append");
    if(radTargetUpdate->isChecked())
        parameters += tr(" -update");
    return parameters;
}

void App::evtMnuOgrHelp( void )
{
    QDesktopServices::openUrl( QUrl( tr( "http://www.gdal.org/ogr2ogr.html" ) ) );
}

void App::evtMnuGuiHelp( void )
{
    QString docPath = tr("file:///") + QCoreApplication::applicationDirPath() + tr("/doc/html/index.html");
    QDesktopServices::openUrl(QUrl(docPath));
}

void App::evtMnuOgrinfo(void) {
    QDesktopServices::openUrl(QUrl(tr("http://gdal.org/ogrinfo.html")));
}

void App::evtMnuHsrAbout( void )
{
    QDesktopServices::openUrl( QUrl( tr( "http://www.hsr.ch/" ) ) );
}

void App::evtMnuOgrAbout( void )
{
    QDesktopServices::openUrl( QUrl( tr( "http://www.ogr2gui.ca/" ) ) );
}

void App::evtRadSourceFile( void )
{
    btnSourceName->setText( tr( "Open" ) );

    cmbSourceFormat->clear();

    for( int i = 0; i < formatsCount; i ++ )
    {
        cmbSourceFormat->addItem( formats[ i ][ 0 ] );
    }

    radTargetFile->setEnabled( true );
    radTargetFolder->setEnabled( false );
    radTargetDatabase->setEnabled( true );

    if(radTargetFolder->isChecked())
        radTargetFile->setChecked( true );

    lblSourceName->setText(tr("Name"));
    txtSourceName->clear();
    txtSourceProj->clear();
    txtSourceQuery->clear();

    txtSourceProj->setEnabled( true );
    txtSourceQuery->setEnabled( true );
}

void App::evtRadSourceFolder( void )
{
    btnSourceName->setText( tr( "Browse" ) );

    cmbSourceFormat->clear();

    for( int i = 0; i < formatsCount; i ++ )
    {
        cmbSourceFormat->addItem( formats[ i ][ 0 ] );
    }

    radTargetFile->setEnabled( false );
    radTargetFolder->setEnabled( true );
    radTargetDatabase->setEnabled( true );

    if(radTargetFile->isChecked())
        radTargetFolder->setChecked( true );

    lblSourceName->setText(tr("Name"));
    txtSourceName->clear();
    txtSourceProj->clear();
    txtSourceQuery->clear();

    txtSourceProj->setEnabled( true );
    txtSourceQuery->setEnabled( true );
}

void App::evtRadSourceDatabase( void )
{
    btnSourceName->setText( tr( "Connect" ) );

    cmbSourceFormat->clear();

    for( int i = 0; i < databasesCount; i ++ )
    {
        cmbSourceFormat->addItem( databases[ i ][ 0 ] );
    }

    radTargetFile->setEnabled( true );
    radTargetFolder->setEnabled( true );
    radTargetDatabase->setEnabled( true );

    lblSourceName->setText(tr("Name"));
    txtSourceName->clear();
    txtSourceProj->clear();
    txtSourceQuery->clear();

    txtSourceProj->setEnabled( true );
    txtSourceQuery->setEnabled( true );
}

void App::evtRadSourceWebservice( void )
{
    btnSourceName->setText( tr( "Connect" ) );

    cmbSourceFormat->clear();

    for( int i = 0; i < webservicesCount; i ++ )
    {
        cmbSourceFormat->addItem( webservices[ i ][ 0 ] );
    }

    radTargetFile->setEnabled( true );
    radTargetFolder->setEnabled( true );
    radTargetDatabase->setEnabled( true );

    lblSourceName->setText(tr("URI"));
    txtSourceProj->clear();
    txtSourceQuery->clear();

    txtSourceProj->setEnabled( true );
    txtSourceQuery->setEnabled( true );
}

void App::evtCmbSourceFormat(void)
{
    txtSourceName->clear();
    txtSourceProj->clear();
    txtSourceQuery->clear();
}

void App::evtTxtSourceName( void ) {
    if( txtSourceName->text().startsWith(tr("file://"))) {
        txtSourceName->setText( QUrl( txtSourceName->text() ).authority().trimmed() );
    }
    string name = txtSourceName->text().trimmed().toStdString();
    string epsg;
    string query;
    string error;

    if(radSourceWebservice->isChecked()) {
        name = webservices[0][1].toStdString() + name;
    }
    txtSourceProj->clear();
    if(ogr->OpenSource(name, epsg, query, error)) {
        for(int i = 0; i < projectionsList.size(); i++) {
            if( strcmp(epsg.c_str(), projectionsList.at(i).first.toStdString().c_str()) == 0) {
                if(i > 1) {
                    txtSourceProj->setText(projectionsList.at(i).first + tr( " : " ) + projectionsList.at(i).second);
                }
                break;
            }
        }
        ogr->CloseSource();
        if( radSourceFile->isChecked()) {
            txtSourceQuery->setText( query.c_str() );
        } else if(radSourceWebservice->isChecked()) {
            btnSourceName->setText(tr("Connected"));
        }
    } else {
        txtSourceProj->clear();
        txtSourceQuery->clear();
        if( radSourceWebservice->isChecked() ) {
            btnSourceName->setText(tr("Connect"));
        }
    }
    UpdateParameters();
}

void App::evtBtnSourceName( void )
{
    txtSourceName->selectAll();
    txtSourceName->setFocus();
    int idx = cmbSourceFormat->currentIndex();

    QString type;

    if( radSourceFile->isChecked() )
    {
        type = tr( "\"" ) + formats[ idx ][ 0 ] + tr( " (*." ) + formats[ idx ][ 1 ] + tr(")\"");
        txtSourceName->setText( QDir::toNativeSeparators(QFileDialog::getOpenFileName( this, tr( "Source File" ), tr( "" ), type ) ));
    }
    else if( radSourceFolder->isChecked() )
    {
        QStringList types;
        type = tr( "*." ) + formats[ cmbSourceFormat->currentIndex() ][ 1 ];

        txtSourceName->setText( QDir::toNativeSeparators(QFileDialog::getExistingDirectory( this, tr( "Source Folder" ), tr( "" ), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks ) ));
        QDir dir( txtSourceName->text() );

        types.append( type );
        QStringList list = dir.entryList( types );

        if( list.size() > 1 )
        {
            txtSourceProj->setEnabled( false );
            txtSourceQuery->setEnabled( false );
        }
    }
    else if( radSourceDatabase->isChecked() )
    {
        inf->setConnectionType(databases[idx][1]);
        inf->showTables(true);
        if( inf->exec() == QDialog::Accepted )
        {
            txtSourceName->setText( inf->getConnectionString() );
        }

        QStringList fileList;

        QStringList tables = inf->getSelectedTables();

        QString connectionString = txtSourceName->text();
        connectionString.truncate( connectionString.lastIndexOf( tr( "tables=" ) ) );

        for( int i = 0; i < tables.size(); ++i)
        {
            fileList.append( connectionString + tr( "tables=" ) + tables.at( i ) );
        }
        if( fileList.size() > 1 )
        {
            txtSourceProj->setEnabled( false );
            txtSourceQuery->setEnabled( false );
        }
    } else if(radSourceWebservice->isChecked()) {
        wfs->setConnectionType(webservices[idx][1]);
        if(wfs->exec() == QDialog::Accepted) {
            txtSourceName->setText(wfs->getConnectionString());
        }
    }
    UpdateParameters();
}

void App::evtRadTargetFile( void )
{
    btnTargetName->setText( tr( "Save" ) );

    cmbTargetFormat->clear();
    for( int i = 0; i < formatsOutput; ++i )
    {
        cmbTargetFormat->addItem( formats[ i ][ 0 ] );
    }

    txtTargetName->clear();
    txtTargetProj->clear();

    cmbTargetProj->setCurrentIndex( 0 );
}

void App::evtRadTargetFolder( void )
{
    btnTargetName->setText( tr( "Browse" ) );

    cmbTargetFormat->clear();
    for( int i = 0; i < formatsOutput; ++i )
    {
        cmbTargetFormat->addItem( formats[ i ][ 0 ] );
    }
}

void App::evtRadTargetDatabase( void )
{
    btnTargetName->setText( tr( "Connect" ) );

    cmbTargetFormat->clear();
    for( int i = 0; i < databasesOutput; ++i )
    {
        cmbTargetFormat->addItem( databases[ i ][ 0 ] );
    }
}

void App::evtCmbTargetFormat( void )
{
    txtTargetName->clear();
    UpdateParameters();
}

void App::evtTxtTargetName( void )
{
    btnExecute->setEnabled( true );
}

void App::evtBtnTargetName( void )
{
    QString type;

    int idx = cmbTargetFormat->currentIndex();

    if( radTargetDatabase->isChecked() )
    {
        if(databases[idx][0] == "SQLite") {
            type = tr("\"") + databases[idx][0] + tr(" (*") + tr(".sqlite") + tr(")\"");
            txtTargetName->setText(QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, tr("Save File"), tr(""), type )));
            UpdateParameters();
            return;
        }
        inf->showTables(false);
        inf->setConnectionType( databases[ cmbTargetFormat->currentIndex() ][ 1 ] );
        if( inf->exec() == QDialog::Accepted )
        {
            txtTargetName->setText( inf->getConnectionString() );
        }
    }
    else if( radTargetFolder->isChecked() )
    {
        if( radSourceFile->isChecked() )
        {
            type = tr( "\"" ) + formats[ idx ][ 0 ] + tr( " (*." ) + formats[ idx ][ 1 ] + tr( ") | *." ) + formats[ idx ][ 1 ];

            txtTargetName->setText( QDir::toNativeSeparators(QFileDialog::getSaveFileName( this, tr( "Save File" ), tr( "" ), type ) ));
        }
        else if( radTargetFolder->isChecked() )
        {
            txtTargetName->setText( QDir::toNativeSeparators(QFileDialog::getExistingDirectory( this, tr( "Target Folder" ), tr( "" ), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks ) ));
        }
    }
    else
    {
        type = tr( "\"" ) + formats[ idx ][ 0 ] + tr( " (*." ) + formats[ idx ][ 1 ] + tr(")\"");

        txtTargetName->setText( QDir::toNativeSeparators(QFileDialog::getSaveFileName( this, tr( "Target File" ), tr( "" ), type ) ));
    }

    btnExecute->setEnabled( true );
    UpdateParameters();
}

void App::evtTxtTargetProj( void )
{
    QString projection = txtTargetProj->text();
    for( int i = 0; i < projectionsList.size(); ++i) {
        if(projectionsList.at(i).first.startsWith(projection)) {
            cmbTargetProj->setCurrentIndex(i);
            break;
        }
    }
    UpdateParameters();
}

void App::evtUpdateParameters( void )
{
    UpdateParameters();
}

void App::evtBtnExecute( void )
{
    UpdateParameters();

    QString sourcename = txtSourceName->text().trimmed();
    QString targetname = txtTargetName->text().trimmed();
    string epsg;
    string query;
    string error;

    if(txtTargetName->text().isEmpty()) {
        txtOutput->append(tr("\n * unable to open target !\n"));
        return;
    }
    bool resVal = true;
    if(radSourceWebservice->isChecked()) {
        QStringList fileList = wfs->getSelectedLayersAsList();
        sourcename = webservices[cmbSourceFormat->currentIndex()][1] + sourcename;
        for(int i=0;i<fileList.size();++i) {
            if(!ogr->OpenSource(sourcename.toStdString(), fileList.at(i).toStdString(), epsg, query, error)) {
                resVal = false;
                break;
            }
        }
    } else {
        resVal = ogr->OpenSource(sourcename.toStdString(), epsg, query, error);
    }
    if(resVal) {
        if(ogr->OpenDriver(cmbTargetFormat->currentText().toStdString())) {
            if(!txtSourceQuery->text().isEmpty()) {
                if(!ogr->TestExecuteSQL(txtSourceQuery->text().toStdString())) {
                    txtOutput->append(tr("\n * unable to execute sql query !\n"));
                    return;
                }
            }
            if(!ogr->TestSpatialReference((projectionsList.at(cmbTargetProj->currentIndex()).first).toInt()))
                txtOutput->append(tr("\n * unable to create spatial reference !\n"));
            if(!radSourceDatabase->isChecked() && !radSourceWebservice->isChecked())
                if(!ogr->TestFeatureProjection())
                    txtOutput->append(tr("\n * unable to transform feature with projection !\n"));

            theProgress->setValue(0);
            theProgress->setMinimum(0);
            theProgress->setMaximum(0);

            txtOutput->append(tr("\n") + sourcename + tr(" > ") + targetname + tr(" ... "));
            QString parameters = currentParameters();
            QString command = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
            if(radSourceWebservice->isChecked())
                parameters += tr(" ") + wfs->getSelectedLayers();
            if(!txtInput->toPlainText().isEmpty())
                parameters += tr(" ") + txtInput->toPlainText();
            command += parameters;
            if(ogr->OpenOgr2ogr(command, btnExecute)) {
                theProgress->setValue(100);
                theProgress->setMaximum(100);
            } else {
                txtOutput->append(tr("\n * unable to open ogr2ogr !\n"));
                theProgress->setValue(0);
                theProgress->setMaximum(0);
            }
        } else {
            txtOutput->append(tr("\n * unable to open driver !\n"));
        }
    } else {
        txtOutput->append(tr("\n * unable to open source !\n"));
    }
}

void App::evtBtnQuit( void )
{
    this->close();
}
