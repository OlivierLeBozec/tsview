#include "mainwindow.h"
#include "thread.h"

MainWindow::MainWindow() :
    m_tsFile(Q_NULLPTR),
    m_pthreadPool(QThreadPool::globalInstance()),
    m_pcrWorker(Q_NULLPTR), m_ptsWorker(Q_NULLPTR), m_dtsWorker(Q_NULLPTR),
    m_pcrDeltaWorker(Q_NULLPTR), m_jitterPcrWorker(Q_NULLPTR), m_ptsDeltaWorker(Q_NULLPTR),
    m_dtsDeltaWorker(Q_NULLPTR), m_diffPcrPtsWorker(Q_NULLPTR), m_diffPcrDtsWorker(Q_NULLPTR),
    m_diffPtsDtsWorker(Q_NULLPTR)
{
    QWidget *main_widget = new QWidget;
    setCentralWidget(main_widget);

    createMenu();
    createLayout(main_widget);
}

MainWindow::~MainWindow()
{
    cleanAll();
}

void MainWindow::cleanAll()
{
    if (m_tsFile) {
        m_tsFile->close();
        delete m_tsFile;
    }
    cleanPcr();
    cleanDts();
    cleanPts();
}

void MainWindow::cleanPcr()
{
    if (m_pcrWorker) {
        m_pthreadPool->cancel(m_pcrWorker);
        delete m_pcrWorker; m_pcrWorker = Q_NULLPTR;
    }
    if (m_pcrDeltaWorker) {
        m_pthreadPool->cancel(m_pcrDeltaWorker);
        delete m_pcrDeltaWorker; m_pcrDeltaWorker = Q_NULLPTR;
    }
    if (m_jitterPcrWorker) {
        m_pthreadPool->cancel(m_jitterPcrWorker);
        delete m_jitterPcrWorker; m_jitterPcrWorker = Q_NULLPTR;
    }
    if (m_diffPcrPtsWorker) {
        m_pthreadPool->cancel(m_diffPcrPtsWorker);
        delete m_diffPcrPtsWorker; m_diffPcrPtsWorker = Q_NULLPTR;
    }
    if (m_diffPcrDtsWorker) {
        m_pthreadPool->cancel(m_diffPcrDtsWorker);
        delete m_diffPcrDtsWorker; m_diffPcrDtsWorker = Q_NULLPTR;
    }
}

void MainWindow::cleanPts()
{
    if (m_ptsWorker) {
        m_pthreadPool->cancel(m_ptsWorker);
        delete m_ptsWorker; m_ptsWorker = Q_NULLPTR;
    }
    if (m_ptsDeltaWorker) {
        m_pthreadPool->cancel(m_ptsDeltaWorker);
        delete m_ptsDeltaWorker; m_ptsDeltaWorker = Q_NULLPTR;
    }
    if (m_diffPcrPtsWorker) {
        m_pthreadPool->cancel(m_diffPcrPtsWorker);
        delete m_diffPcrPtsWorker; m_diffPcrPtsWorker = Q_NULLPTR;
    }
    if (m_diffPtsDtsWorker) {
        m_pthreadPool->cancel(m_diffPtsDtsWorker);
        delete m_diffPtsDtsWorker; m_diffPtsDtsWorker = Q_NULLPTR;
    }
}

void MainWindow::cleanDts()
{
    if (m_dtsWorker) {
        m_pthreadPool->cancel(m_dtsWorker);
        delete m_dtsWorker; m_dtsWorker = Q_NULLPTR;
    }
    if (m_dtsDeltaWorker) {
        m_pthreadPool->cancel(m_dtsDeltaWorker);
        delete m_dtsDeltaWorker; m_dtsDeltaWorker = Q_NULLPTR;
    }
    if (m_diffPcrDtsWorker) {
        m_pthreadPool->cancel(m_diffPcrDtsWorker);
        delete m_diffPcrDtsWorker; m_diffPcrDtsWorker = Q_NULLPTR;
    }
    if (m_diffPtsDtsWorker) {
        m_pthreadPool->cancel(m_diffPtsDtsWorker);
        delete m_diffPtsDtsWorker; m_diffPtsDtsWorker = Q_NULLPTR;
    }
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("viewTs version 0.90 for Windows and Linux\n\n"
               "Source code : https://github.com/OlivierLeBozec/tstools\n"
               "Report issues : https://github.com/OlivierLeBozec/tstools/issues\n"
               "Icon from http://www.flaticon.com/authors/gregor-cresnar\n"));
}

void MainWindow::updateStatusBar(int percent)
{
    //qDebug() << "updateStatusBar " << percent;

    if (percent == 100) {
        statusBar()->showMessage(tr("Done..."), 5000);
    }
    else {
        QString message = QString(tr("Processing %1%")).arg(QString::number(percent));
        statusBar()->showMessage(message, 1000);
    }
}

void MainWindow::createLayout(QWidget *widget)
{
    // pcr
    m_pcrGroupBox = new QGroupBox(tr("Pcr"));
    QLabel* pcrPidLabel = new QLabel(tr("Pid:"));
    m_pcrComboBox = new QComboBox;
    m_pcrBox = new QCheckBox(tr("Display Pcr"));
    m_deltaPcrBox = new QCheckBox(tr("Display Pcr Delta"));
    m_jitterPcrBox = new QCheckBox(tr("Display Pcr Jitter"));

    QGridLayout *pcrGroupBoxLayout = new QGridLayout;
    pcrGroupBoxLayout->addWidget(pcrPidLabel, 0, 0);
    pcrGroupBoxLayout->addWidget(m_pcrComboBox, 0, 1);
    pcrGroupBoxLayout->addWidget(m_pcrBox, 1, 0);
    pcrGroupBoxLayout->addWidget(m_deltaPcrBox, 2, 0);
    pcrGroupBoxLayout->addWidget(m_jitterPcrBox, 3, 0);
    pcrGroupBoxLayout->setVerticalSpacing(0);
    m_pcrGroupBox->setLayout(pcrGroupBoxLayout);
    m_pcrGroupBox->setEnabled(false);

    connect(m_pcrBox, SIGNAL(stateChanged(int)), this, SLOT(Pcr(int)));
    connect(m_deltaPcrBox, SIGNAL(stateChanged(int)), this, SLOT(deltaPcr(int)));
    connect(m_jitterPcrBox, SIGNAL(stateChanged(int)), this, SLOT(jitterPcr(int)));
    connect(m_pcrComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(erasePcrSeries(int)));

    // pts
    m_ptsGroupBox = new QGroupBox(tr("Pts"));
    QLabel* ptsPidLabel = new QLabel(tr("Pid:"));
    m_ptsComboBox = new QComboBox;
    m_ptsBox = new QCheckBox(tr("Display Pts"));
    m_deltaPtsBox = new QCheckBox(tr("Display Pts Delta"));

    QGridLayout *ptsGroupBoxLayout = new QGridLayout;
    ptsGroupBoxLayout->addWidget(ptsPidLabel, 0, 0);
    ptsGroupBoxLayout->addWidget(m_ptsComboBox, 0, 1);
    ptsGroupBoxLayout->addWidget(m_ptsBox, 1, 0);
    ptsGroupBoxLayout->addWidget(m_deltaPtsBox, 2, 0);
    ptsGroupBoxLayout->setVerticalSpacing(0);
    m_ptsGroupBox->setLayout(ptsGroupBoxLayout);
    m_ptsGroupBox->setEnabled(false);

    connect(m_ptsBox, SIGNAL(stateChanged(int)), this, SLOT(Pts(int)));
    connect(m_deltaPtsBox, SIGNAL(stateChanged(int)), this, SLOT(deltaPts(int)));
    connect(m_ptsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(erasePtsSeries(int)));

    // dts
    m_dtsGroupBox = new QGroupBox(tr("Dts"));
    QLabel* dtsPidLabel = new QLabel(tr("Pid:"));
    m_dtsComboBox = new QComboBox;
    m_dtsBox = new QCheckBox(tr("Display Dts"));
    m_deltaDtsBox = new QCheckBox(tr("Display Dts Delta"));

    QGridLayout *dtsGroupBoxLayout = new QGridLayout;
    dtsGroupBoxLayout->addWidget(dtsPidLabel, 0, 0, Qt::AlignTop);
    dtsGroupBoxLayout->addWidget(m_dtsComboBox, 0, 1, Qt::AlignTop);
    dtsGroupBoxLayout->addWidget(m_dtsBox, 1, 0, Qt::AlignTop);
    dtsGroupBoxLayout->addWidget(m_deltaDtsBox, 2, 0, Qt::AlignTop);
    dtsGroupBoxLayout->setVerticalSpacing(0);
    m_dtsGroupBox->setLayout(dtsGroupBoxLayout);
    m_dtsGroupBox->setEnabled(false);

    connect(m_dtsBox, SIGNAL(stateChanged(int)), this, SLOT(Dts(int)));
    connect(m_deltaDtsBox, SIGNAL(stateChanged(int)), this, SLOT(deltaDts(int)));
    connect(m_dtsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(eraseDtsSeries(int)));

    // diff
    m_diffGroupBox = new QGroupBox(tr("Diff"));
    m_diffPcrPtsBox = new QCheckBox(tr("Display Pts-Pcr"));
    m_diffPtsDtsBox = new QCheckBox(tr("Display Dts-Pts"));
    m_diffPcrDtsBox = new QCheckBox(tr("Display Dts-Pcr"));

    QGridLayout *diffGroupBoxLayout = new QGridLayout;
    diffGroupBoxLayout->addWidget(m_diffPcrPtsBox, 0, 0, Qt::AlignTop);
    diffGroupBoxLayout->addWidget(m_diffPtsDtsBox, 1, 0, Qt::AlignTop);
    diffGroupBoxLayout->addWidget(m_diffPcrDtsBox, 2, 0, Qt::AlignTop);
    diffGroupBoxLayout->setVerticalSpacing(0);
    m_diffGroupBox->setLayout(diffGroupBoxLayout);
    m_diffGroupBox->setEnabled(false);

    connect(m_diffPcrPtsBox, SIGNAL(stateChanged(int)), this, SLOT(diffPcrPts(int)));
    connect(m_diffPcrDtsBox, SIGNAL(stateChanged(int)), this, SLOT(diffPcrDts(int)));
    connect(m_diffPtsDtsBox, SIGNAL(stateChanged(int)), this, SLOT(diffPtsDts(int)));

    // graph
    Chart *chart = new Chart;
    chart->setTitle("");
    chart->setAnimationOptions(QChart::NoAnimation);
    chart->setTheme(QChart::ChartThemeDark);

    m_chartView = new ChartView(chart);
    //m_chartView->setRenderHint(QPainter::Antialiasing);

    // vertical layout on left hand side
    QBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->setSizeConstraint(QLayout::SetNoConstraint);
    controlLayout->addWidget(m_pcrGroupBox);
    controlLayout->addWidget(m_ptsGroupBox);
    controlLayout->addWidget(m_dtsGroupBox);
    controlLayout->addWidget(m_diffGroupBox);
    controlLayout->addStretch(); // to add space when resize

    // horizontal layout
    QBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addLayout(controlLayout);
    hlayout->addWidget(m_chartView);

    // apply layout
    widget->setLayout(hlayout);
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAct);

    QAction *saveAsAct = new QAction(tr("&SaveAs..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save series in file"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);
    fileMenu->addAction(saveAsAct);

    QMenu *graphMenu = menuBar()->addMenu(tr("&Graph"));
    QAction *clearAllAct = new QAction(tr("&Clear All"), this);
    clearAllAct->setShortcuts(QKeySequence::Delete);
    clearAllAct->setStatusTip(tr("Clear all data"));
    connect(clearAllAct, &QAction::triggered, this, &MainWindow::clearAllSeries);
    graphMenu->addAction(clearAllAct);

    QMenu *helpMenu = menuBar()->addMenu(tr("&About"));
    QAction *aboutAct = helpMenu->addAction(tr("&About..."), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application About box"));
}

void MainWindow::erasePcrSeries(int)
{
    m_pcrBox->setChecked(false);
    m_deltaPcrBox->setChecked(false);
    m_jitterPcrBox->setChecked(false);
    m_diffPcrPtsBox->setChecked(false);
    m_diffPcrDtsBox->setChecked(false);
    cleanPcr();
}

void MainWindow::erasePtsSeries(int)
{
    m_ptsBox->setChecked(false);
    m_deltaPtsBox->setChecked(false);
    m_diffPcrPtsBox->setChecked(false);
    m_diffPtsDtsBox->setChecked(false);
    cleanPts();
}

void MainWindow::eraseDtsSeries(int)
{
    m_dtsBox->setChecked(false);
    m_deltaDtsBox->setChecked(false);
    m_diffPtsDtsBox->setChecked(false);
    m_diffPcrDtsBox->setChecked(false);
    cleanDts();
}

void MainWindow::clearAllSeries()
{
    erasePcrSeries(0);
    eraseDtsSeries(0);
    erasePtsSeries(0);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Ts"), NULL);
    if (!fileName.isEmpty())
    {
        // delete previous allocations
        //statusBar()->showMessage(tr("Clean all..."));
        //cleanAll();
        clearAllSeries();

        statusBar()->showMessage(tr("Loading file..."));
        m_tsFile = new std::ifstream(fileName.toStdString().c_str(), std::ios::binary);

        statusBar()->showMessage(tr("Parsing first 5000 packets..."));
        pidmap pm(*m_tsFile);
        pm.run(5000);

        // clear previous value
        m_pcrComboBox->clear();
        m_ptsComboBox->clear();
        m_dtsComboBox->clear();
        m_pcrGroupBox->setEnabled(false);
        m_ptsGroupBox->setEnabled(false);
        m_dtsGroupBox->setEnabled(false);
        m_diffGroupBox->setEnabled(false);

        // update pcr/pts/dts pid
        std::vector<unsigned int>::iterator it;
        std::vector<unsigned int> pidVect;

        bool hasPcr = false;
        pm.getPcrPid(pidVect);
        for (it = pidVect.begin(); it < pidVect.end(); it++)
        {
            hasPcr = true;
            m_pcrComboBox->addItem(QString::number(*it), *it);
            m_pcrGroupBox->setEnabled(true);
        }
        pidVect.clear();

        bool hasPts = false;
        pm.getPtsPid(pidVect);
        for (it = pidVect.begin(); it < pidVect.end(); it++)
        {
            hasPts = true;
            m_ptsComboBox->addItem(QString::number(*it), *it);
            m_ptsGroupBox->setEnabled(true);
        }
        pidVect.clear();

        bool hasDts = false;
        pm.getDtsPid(pidVect);
        for (it = pidVect.begin(); it < pidVect.end(); it++)
        {
            hasDts = true;
            m_dtsComboBox->addItem(QString::number(*it), *it);
            m_dtsGroupBox->setEnabled(true);
        }
        pidVect.clear();

        if (hasPcr && hasPts && hasDts)
        {
            m_diffGroupBox->setEnabled(true);
        }
        else if (hasPcr && hasPts)
        {
            // no Dts
            m_diffGroupBox->setEnabled(true);
            m_diffPtsDtsBox->setEnabled(false);
            m_diffPcrDtsBox->setEnabled(false);
        }
        else if (hasDts && hasPts)
        {
            // no Pcr
            m_diffGroupBox->setEnabled(true);
            m_diffPcrPtsBox->setEnabled(false);
            m_diffPcrDtsBox->setEnabled(false);
        }

        statusBar()->showMessage(tr("Done..."), 1000);
    }
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save series"), NULL, NULL /*tr("*.txt")*/);
    if (!fileName.isEmpty())
    {
        // overwrite all in the file
        statusBar()->showMessage(tr("Saving file..."));
        std::ofstream* outFile = new std::ofstream(fileName.toStdString().c_str(), std::ofstream::out | std::ofstream::trunc);

        serializeSeries(outFile, m_pcrWorker);
        serializeSeries(outFile, m_ptsWorker);
        serializeSeries(outFile, m_dtsWorker);
        serializeSeries(outFile, m_pcrDeltaWorker);
        serializeSeries(outFile, m_jitterPcrWorker);
        serializeSeries(outFile, m_ptsDeltaWorker);
        serializeSeries(outFile, m_dtsDeltaWorker);
        serializeSeries(outFile, m_diffPcrPtsWorker);
        serializeSeries(outFile, m_diffPcrDtsWorker);
        serializeSeries(outFile, m_diffPtsDtsWorker);

        outFile->close();
        statusBar()->showMessage(tr("Done..."), 1000);
    }
}

void MainWindow::showSeries(timeStampWorker *pWorker)
{
    if (pWorker->m_isRunning) {
        // if thread has not ended then reconnect the signal
        connect(pWorker, SIGNAL(updated(int)), this, SLOT(updateStatusBar(int)));
    }
    else {
        // redraw serie only
        pWorker->showSeries();
    }
}

void MainWindow::hideSeries(timeStampWorker *pWorker)
{
    if (pWorker->m_isRunning) {
        // if thread has not ended then disconnect the signal
        disconnect(pWorker, SIGNAL(updated(int)), this, SLOT(updateStatusBar(int)));
    }
    else {
        // undraw serie
        pWorker->hideSeries();
    }
}

void MainWindow::buildSeries(timeStampWorker *pWorker)
{
    // build serie
    connect(pWorker, SIGNAL(updated(int)), this, SLOT(updateStatusBar(int)));

    // start thread
    m_pthreadPool->start(pWorker);
}

void MainWindow::serializeSeries(std::ofstream* outFile, timeStampWorker *pWorker)
{
    if (pWorker == NULL || pWorker->m_isRunning == true)
        return;

    // serialize serie in file
    pWorker->serializeSeries(outFile);
}

/////////////////////////////////////////////// Chart
void MainWindow::Pcr(int state)
{
    if (state == Qt::Checked)
        if (m_pcrWorker == Q_NULLPTR)
        {
            unsigned int pid = m_pcrComboBox->itemData(m_pcrComboBox->currentIndex()).toInt();
            m_pcrWorker = new pcrWorker(m_tsFile, pid, (Chart*)m_chartView->chart());
            connect(m_pcrWorker, SIGNAL(finished()), this, SLOT(showPcr()));
            buildSeries(m_pcrWorker);
        }
        else
            showSeries(m_pcrWorker);
    else
        hideSeries(m_pcrWorker);
}

void  MainWindow::showPcr()
{
    if (m_pcrBox->isChecked()) {
        // must be done in parent thread
        m_pcrWorker->showSeries();
    }
}

void MainWindow::Pts(int state)
{
    if (state == Qt::Checked)
        if (m_ptsWorker == Q_NULLPTR)
        {
            unsigned int pid = m_ptsComboBox->itemData(m_ptsComboBox->currentIndex()).toInt();
            m_ptsWorker = new ptsWorker(m_tsFile, pid, (Chart*)m_chartView->chart());
            connect(m_ptsWorker, SIGNAL(finished()), this, SLOT(showPts()));
            buildSeries(m_ptsWorker);
        }
        else
            showSeries(m_ptsWorker);
    else
        hideSeries(m_ptsWorker);
}

void  MainWindow::showPts()
{
    if (m_ptsBox->isChecked()) {
        // must be done in parent thread
        m_ptsWorker->showSeries();
    }
}

void MainWindow::Dts(int state)
{
    if (state == Qt::Checked)
        if (m_dtsWorker == Q_NULLPTR)
        {
            unsigned int pid = m_dtsComboBox->itemData(m_dtsComboBox->currentIndex()).toInt();
            m_dtsWorker = new dtsWorker(m_tsFile, pid, (Chart*)m_chartView->chart());
            connect(m_dtsWorker, SIGNAL(finished()), this, SLOT(updateDts()));
            buildSeries(m_dtsWorker);
        }
        else
            showSeries(m_dtsWorker);
    else
        hideSeries(m_dtsWorker);
}

void  MainWindow::updateDts()
{
    if (m_dtsBox->isChecked()) {
        // must be done in parent thread
        m_dtsWorker->showSeries();
    }
}

void MainWindow::deltaPcr(int state)
{
    if (state == Qt::Checked)
        if (m_pcrDeltaWorker == Q_NULLPTR)
        {
            unsigned int pid = m_pcrComboBox->itemData(m_pcrComboBox->currentIndex()).toInt();
            m_pcrDeltaWorker = new pcrDeltaWorker(m_tsFile, pid, (Chart*)m_chartView->chart());
            connect(m_pcrDeltaWorker, SIGNAL(finished()), this, SLOT(showDeltaPcr()));
            buildSeries(m_pcrDeltaWorker);
        }
        else
            showSeries(m_pcrDeltaWorker);
    else
        hideSeries(m_pcrDeltaWorker);
}

void MainWindow::showDeltaPcr()
{
    if (m_deltaPcrBox->isChecked()) {
        // must be done in parent thread
        m_pcrDeltaWorker->showSeries();
    }
}

void MainWindow::deltaPts(int state)
{
    if (state == Qt::Checked)
        if (m_ptsDeltaWorker == Q_NULLPTR)
        {
            unsigned int pid = m_ptsComboBox->itemData(m_ptsComboBox->currentIndex()).toInt();
            m_ptsDeltaWorker = new ptsDeltaWorker(m_tsFile, pid, (Chart*)m_chartView->chart());
            connect(m_ptsDeltaWorker, SIGNAL(finished()), this, SLOT(showPtsDelta()));
            buildSeries(m_ptsDeltaWorker);
        }
        else
            showSeries(m_ptsDeltaWorker);
    else
        hideSeries(m_ptsDeltaWorker);
}

void  MainWindow::showPtsDelta()
{
    if (m_deltaPtsBox->isChecked()) {
        // must be done in parent thread
        m_ptsDeltaWorker->showSeries();
    }
}


void MainWindow::deltaDts(int state)
{
    if (state == Qt::Checked)
        if (m_dtsDeltaWorker == Q_NULLPTR)
        {
            unsigned int pid = m_dtsComboBox->itemData(m_dtsComboBox->currentIndex()).toInt();
            m_dtsDeltaWorker = new dtsDeltaWorker(m_tsFile, pid, (Chart*)m_chartView->chart());
            connect(m_dtsDeltaWorker, SIGNAL(finished()), this, SLOT(showDtsDelta()));
            buildSeries(m_dtsDeltaWorker);
        }
        else
            showSeries(m_dtsDeltaWorker);
    else
        hideSeries(m_dtsDeltaWorker);
}

void  MainWindow::showDtsDelta()
{
    if (m_deltaDtsBox->isChecked()) {
        // must be done in parent thread
        m_dtsDeltaWorker->showSeries();
    }
}

void MainWindow::jitterPcr(int state)
{
    if (state == Qt::Checked)
        if (m_jitterPcrWorker == Q_NULLPTR)
        {
            unsigned int pid = m_pcrComboBox->itemData(m_pcrComboBox->currentIndex()).toInt();
            m_jitterPcrWorker = new pcrJitterWorker(m_tsFile, pid, (Chart*)m_chartView->chart());
            connect(m_jitterPcrWorker, SIGNAL(finished()), this, SLOT(showJitterPcr()));
            buildSeries(m_jitterPcrWorker);
        }
        else
            showSeries(m_jitterPcrWorker);
    else
        hideSeries(m_jitterPcrWorker);
}

void MainWindow::showJitterPcr()
{
    if (m_jitterPcrBox->isChecked()) {
        // must be done in parent thread
        m_jitterPcrWorker->showSeries();
    }
}

void MainWindow::diffPcrPts(int state)
{
    if (state == Qt::Checked)
        if (m_diffPcrPtsWorker == Q_NULLPTR)
        {
            unsigned int pidPcr = m_pcrComboBox->itemData(m_pcrComboBox->currentIndex()).toInt();
            unsigned int pidPts = m_ptsComboBox->itemData(m_ptsComboBox->currentIndex()).toInt();
            m_diffPcrPtsWorker = new diffPcrPtsWorker(m_tsFile, pidPcr, pidPts, (Chart*)m_chartView->chart());
            connect(m_diffPcrPtsWorker, SIGNAL(finished()), this, SLOT(showDiffPcrPts()));
            buildSeries(m_diffPcrPtsWorker);
        }
        else
            showSeries(m_diffPcrPtsWorker);
    else
        hideSeries(m_diffPcrPtsWorker);
}

void MainWindow::showDiffPcrPts()
{
    if (m_diffPcrPtsBox->isChecked()) {
        // must be done in parent thread
        m_diffPcrPtsWorker->showSeries();
    }
}


void MainWindow::diffPcrDts(int state)
{
    if (state == Qt::Checked)
        if (m_diffPcrDtsWorker == Q_NULLPTR)
        {
            unsigned int pidPcr = m_pcrComboBox->itemData(m_pcrComboBox->currentIndex()).toInt();
            unsigned int pidDts = m_dtsComboBox->itemData(m_dtsComboBox->currentIndex()).toInt();
            m_diffPcrDtsWorker = new diffPcrDtsWorker(m_tsFile, pidPcr, pidDts, (Chart*)m_chartView->chart());
            connect(m_diffPcrDtsWorker, SIGNAL(finished()), this, SLOT(showDiffPcrDts()));
            buildSeries(m_diffPcrDtsWorker);
        }
        else
            showSeries(m_diffPcrDtsWorker);
    else
        hideSeries(m_diffPcrDtsWorker);
}

void MainWindow::showDiffPcrDts()
{
    if (m_diffPcrDtsBox->isChecked()) {
        // must be done in parent thread
        m_diffPcrDtsWorker->showSeries();
    }
}

void MainWindow::diffPtsDts(int state)
{
    if (state == Qt::Checked)
        if (m_diffPtsDtsWorker == Q_NULLPTR)
        {
            unsigned int pidPts = m_ptsComboBox->itemData(m_ptsComboBox->currentIndex()).toInt();
            unsigned int pidDts = m_dtsComboBox->itemData(m_dtsComboBox->currentIndex()).toInt();
            m_diffPtsDtsWorker = new diffPtsDtsWorker(m_tsFile, pidPts, pidDts, (Chart*)m_chartView->chart());
            connect(m_diffPtsDtsWorker, SIGNAL(finished()), this, SLOT(showDiffPtsDts()));
            buildSeries(m_diffPtsDtsWorker);
        }
        else
            showSeries(m_diffPtsDtsWorker);
    else
        hideSeries(m_diffPtsDtsWorker);
}

void MainWindow::showDiffPtsDts()
{
    if (m_diffPtsDtsBox->isChecked()) {
        // must be done in parent thread
        m_diffPtsDtsWorker->showSeries();
    }
}
