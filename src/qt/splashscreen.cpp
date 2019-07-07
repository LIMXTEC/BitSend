// Copyright (c) 2011-2018 The Bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include <config/bitsend-config.h>
#endif

#include <qt/splashscreen.h>

#include <qt/networkstyle.h>

#include <clientversion.h>
#include <interfaces/handler.h>
#include <interfaces/node.h>
#include <interfaces/wallet.h>
#include <util.h>
#include <ui_interface.h>
#include <version.h>

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QPainter>
#include <QRadialGradient>

SplashScreen::SplashScreen(Qt::WindowFlags f, const QPixmap &pixmap) :
    QSplashScreen(pixmap, f)
{
    setAutoFillBackground(true);

    // set reference point, paddings
    int paddingLeft             = 14;
    int paddingTop              = 470;
    int titleVersionVSpace      = 17;
    int titleCopyrightVSpace    = 32;

    float fontFactor            = 1.0;

    // define text to place
    QString titleText       = tr("BitSend Core");
    QString versionText     = QString(tr("Version %1")).arg(QString::fromStdString(FormatFullVersion()));
    QString copyrightTextBtc   = QChar(0xA9)+QString(" 2009-%1 ").arg(COPYRIGHT_YEAR) + QString(tr("The Bitcoin Core developers"));
    QString copyrightTextDrk   = QChar(0xA9)+QString(" 2014-%1 ").arg(COPYRIGHT_YEAR) + QString(tr("The BitSend Core developers"));
    QString testnetAddText  = QString(tr("[testnet]")); // define text to place as single text object

    QString font            = "Arial";

    // load the bitmap for writing some text over it
    QPixmap newPixmap;
    newPixmap     = QPixmap(":/images/splash");
    

    QPainter pixPaint(&newPixmap);
    pixPaint.setPen(QColor(100,100,100));

    // check font size and drawing with
    pixPaint.setFont(QFont(font, 28*fontFactor));
    QFontMetrics fm = pixPaint.fontMetrics();
    int titleTextWidth  = fm.width(titleText);
    if(titleTextWidth > 160) {
        // strange font rendering, Arial probably not found
        fontFactor = 0.75;
    }

    pixPaint.setFont(QFont(font, 28*fontFactor));
    fm = pixPaint.fontMetrics();
    titleTextWidth  = fm.width(titleText);
    pixPaint.drawText(paddingLeft,paddingTop,titleText);

    pixPaint.setFont(QFont(font, 15*fontFactor));
    pixPaint.drawText(paddingLeft,paddingTop+titleVersionVSpace,versionText);

    // draw copyright stuff
    pixPaint.setFont(QFont(font, 10*fontFactor));
    pixPaint.drawText(paddingLeft,paddingTop+titleCopyrightVSpace,copyrightTextBtc);
    pixPaint.drawText(paddingLeft,paddingTop+titleCopyrightVSpace+12,copyrightTextDrk);

    // draw testnet string if testnet is on
    /* if(isTestNet) {
        QFont boldFont = QFont(font, 10*fontFactor);
        boldFont.setWeight(QFont::Bold);
        pixPaint.setFont(boldFont);
        fm = pixPaint.fontMetrics();
        int testnetAddTextWidth  = fm.width(testnetAddText);
        pixPaint.drawText(newPixmap.width()-testnetAddTextWidth-10,newPixmap.height()-25,testnetAddText);
    } */

    pixPaint.end();

    this->setPixmap(newPixmap);

    subscribeToCoreSignals();
}

SplashScreen::~SplashScreen()
{
    unsubscribeFromCoreSignals();
}

bool SplashScreen::eventFilter(QObject * obj, QEvent * ev) {
    if (ev->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
        if(keyEvent->text()[0] == 'q') {
            m_node.startShutdown();
        }
    }
    return QObject::eventFilter(obj, ev);
}

void SplashScreen::slotFinish(QWidget *mainWin)
{
    Q_UNUSED(mainWin);

    /* If the window is minimized, hide() will be ignored. */
    /* Make sure we de-minimize the splashscreen window before hiding */
    if (isMinimized())
        showNormal();
    hide();
    deleteLater(); // No more need for this
}

static void InitMessage(SplashScreen *splash, const std::string &message)
{
    QMetaObject::invokeMethod(splash, "showMessage",
        Qt::QueuedConnection,
        Q_ARG(QString, QString::fromStdString(message)),
        Q_ARG(int, Qt::AlignBottom|Qt::AlignHCenter),
        Q_ARG(QColor, QColor(55,55,55)));
}

static void ShowProgress(SplashScreen *splash, const std::string &title, int nProgress, bool resume_possible)
{
    InitMessage(splash, title + std::string("\n") +
            (resume_possible ? _("(press q to shutdown and continue later)")
                                : _("press q to shutdown")) +
            strprintf("\n%d", nProgress) + "%");
}
#ifdef ENABLE_WALLET
void SplashScreen::ConnectWallet(std::unique_ptr<interfaces::Wallet> wallet)
{
    m_connected_wallet_handlers.emplace_back(wallet->handleShowProgress(boost::bind(ShowProgress, this, _1, _2, false)));
    m_connected_wallets.emplace_back(std::move(wallet));
}
#endif

void SplashScreen::subscribeToCoreSignals()
{
    // Connect signals to client
    m_handler_init_message = m_node.handleInitMessage(boost::bind(InitMessage, this, _1));
    m_handler_show_progress = m_node.handleShowProgress(boost::bind(ShowProgress, this, _1, _2, _3));
#ifdef ENABLE_WALLET
    m_handler_load_wallet = m_node.handleLoadWallet([this](std::unique_ptr<interfaces::Wallet> wallet) { ConnectWallet(std::move(wallet)); });
#endif
}

void SplashScreen::unsubscribeFromCoreSignals()
{
    // Disconnect signals from client
    m_handler_init_message->disconnect();
    m_handler_show_progress->disconnect();
    for (auto& handler : m_connected_wallet_handlers) {
        handler->disconnect();
    }
    m_connected_wallet_handlers.clear();
    m_connected_wallets.clear();
}

void SplashScreen::showMessage(const QString &message, int alignment, const QColor &color)
{
    curMessage = message;
    curAlignment = alignment;
    curColor = color;
    update();
}

void SplashScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    QRect r = rect().adjusted(5, 5, -5, -5);
    painter.setPen(curColor);
    painter.drawText(r, curAlignment, curMessage);
}

void SplashScreen::closeEvent(QCloseEvent *event)
{
    m_node.startShutdown(); // allows an "emergency" shutdown during startup
    event->ignore();
}
