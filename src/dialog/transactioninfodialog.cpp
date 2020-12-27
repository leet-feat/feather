// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020-2021, The Monero Project.

#include "transactioninfodialog.h"
#include "ui_transactioninfodialog.h"

#include "libwalletqt/CoinsInfo.h"
#include "libwalletqt/WalletManager.h"
#include "utils.h"

#include <QMessageBox>

TransactionInfoDialog::TransactionInfoDialog(Wallet *wallet, TransactionInfo *txInfo, QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::TransactionInfoDialog)
        , m_wallet(wallet)
        , m_txInfo(txInfo)
{
    ui->setupUi(this);

    m_txProofWidget = new TxProofWidget(this, wallet, txInfo);

    ui->label_txid->setText(QString(txInfo->hash()));

    if (txInfo->direction() == TransactionInfo::Direction_In) {
        ui->txKey->hide();
    } else {
        QString txKey = m_wallet->getTxKey(txInfo->hash());
        txKey = txKey.isEmpty() ? "unknown" : txKey;
        ui->label_txKey->setText(txKey);
    }

    QString blockHeight = QString::number(txInfo->blockHeight());
    if (blockHeight == "0")
        blockHeight = "Unconfirmed";

    ui->label_status->setText(QString("Status: %1 confirmations").arg(txInfo->confirmations()));
    ui->label_date->setText(QString("Date: %1").arg(txInfo->timestamp().toString("yyyy-MM-dd HH:mm")));
    ui->label_blockHeight->setText(QString("Block height: %1").arg(blockHeight));

    QString direction = txInfo->direction() == TransactionInfo::Direction_In ? "received" : "sent";
    ui->label_amount->setText(QString("Amount %1: %2").arg(direction, txInfo->displayAmount()));

    QString fee = txInfo->fee().isEmpty() ? "n/a" : txInfo->fee();
    ui->label_fee->setText(QString("Fee: %1").arg(txInfo->isCoinbase() ? WalletManager::displayAmount(0) : fee));
    ui->label_unlockTime->setText(QString("Unlock time: %1 (height)").arg(txInfo->unlockTime()));

    qDebug() << m_wallet->coins()->coins_from_txid(txInfo->hash());

    QString destinations = txInfo->destinations_formatted();
    if (destinations.isEmpty()) {
        ui->frameDestinations->hide();
    } else {
        ui->destinations->setText(destinations);
    }

    ui->txProofWidget->addWidget(m_txProofWidget);

    this->adjustSize();
}

TransactionInfoDialog::~TransactionInfoDialog() {
    delete ui;
}
