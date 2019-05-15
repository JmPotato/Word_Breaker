#include "game.h"

Game::Game(QObject *parent): Client(parent) {
    socket = new QUdpSocket(this);
    socket->bind(clientAddress, clientPort+1);

    difficulty = 2;

    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

short Game::send(const QByteArray data) {
    try {
        socket->writeDatagram(data.data(), data.size(), QHostAddress::LocalHost, 4321);
        return 1;
    } catch (...) {
        return -1;
    }
}

void Game::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        Packet recPacket;
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());
        unpackPacket(datagram, recPacket);
        cout << recPacket.signalType << endl;
        switch(recPacket.signalType) {
            case UPDATE_WORD_RESPONSE:
            case -UPDATE_WORD_RESPONSE:
                cout << "UPDATE WORD RESPONSE" << endl;
                emit updateWordSignal(recPacket);
                break;
            case GET_WROD_RESPONSE:
            case -GET_WROD_RESPONSE:
                cout << "GET WORD RESPONSE" << endl;
                emit getWordSignal(recPacket);
                break;
            default:
                cout << "FAILED RESPONSE" << endl;
                break;
        }
    }
}

void Game::updateWord(string _word) {
    QByteArray data;
    Packet word;
    word.signalType = 1;
    strcpy(word.word, _word.c_str());
    packPacket(data, word);
    send(data);
}

void Game::getWord(unsigned short wordLength) {
    QByteArray data;
    Packet word;
    word.signalType = 2;
    word.length = wordLength;
    packPacket(data, word);
    send(data);
}

void Game::endGame() {
    difficulty = 2;
    currentLevel = 1;
    currentTimeLimted = 5;
    wordList.clear();
}
