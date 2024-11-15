#!/usr/bin/node
const net = require('net');
const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})

const sock = new net.Socket();

function test(sock) {
    rl.question("Отправьте функцию с числом(c дробью):", text => {
        if (text === "\n") {
            sock.write('\n');
            sock.end();
        } else {
            const a = text.match(/^(sin|cos|tg|ctg)\s+(\d+\.\d+)?$/);
            if (a) {
                sock.write(JSON.stringify({
                    method: a[1],
                    value: parseFloat(a[2])
                }));
            } else {
                console.log('Некорректно указали функцию или число');
                test(sock);
            }
        }
    });
}

sock.connect(8886, 'localhost', () => {
    console.log('Соединение установлено!');
    test(sock);
});
sock.on('data', data => {
    console.log(data.toString());
    test(sock);
});
sock.on('close', () => {
    console.log("Соединение закрыто!");
    process.exit();
});