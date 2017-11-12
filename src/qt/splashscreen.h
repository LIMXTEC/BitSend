// Copyright (c) 2011-2016 The Bitsend Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITSEND_QT_SPLASHSCREEN_H
#define BITSEND_QT_SPLASHSCREEN_H

#include <QSplashScreen>

class CWallet;
class NetworkStyle;

/** Class for the splashscreen with information of the running client.
 *
 * @note this is intentionally not a QSplashScreen. Bitsend Core initialization
 * can take a long time, and in that case a progress window that cannot be
 * moved around and minimized has turned out to be frustrating to the user.
 */
class SplashScreen : public QSplashScreen/* QWidget */
{
    Q_OBJECT

public:
    explicit SplashScreen(Qt::WindowFlags f, const QPixmap &pixmap);
    ~SplashScreen();

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

public Q_SLOTS:
    /** Slot to call finish() method as it's not defined as slot */
    void slotFinish(QWidget *mainWin);

    /** Show message and progress */
    void showMessage(const QString &message, int alignment, const QColor &color);

private:
    /** Connect core signals to splash screen */
    void subscribeToCoreSignals();
    /** Disconnect core signals to splash screen */
    void unsubscribeFromCoreSignals();
    /** Connect wallet signals to splash screen */
    void ConnectWallet(CWallet*);

    QPixmap pixmap;
    QString curMessage;
    QColor curColor;
    int curAlignment;

    QList<CWallet*> connectedWallets;
};

#endif // BITSEND_QT_SPLASHSCREEN_H
