/*
const {onExit} = require('@rauschma/stringio');
const {spawn} = require('child_process');
async function main() {
	const filePath = process.argv[2];
	console.log('INPUT: '+filePath);
	const childProcess = spawn('cat', [filePath],{stdio: [process.stdin, process.stdout, process.stderr]})
	await onExit(childProcess); // (B)
	console.log('### DONE');
}
*/

var resultTH = "hello";

var http = require('http');

var server = http.createServer(function(request, response) {

	const { execFile } = require('child_process');
	const child = execFile('/opt/orangepizero-temperature/htu21-sensor/build/htu21-orangepi-test.elf', ['a'], (error, stdout, stderr) => {
	// if (error) {throw error;}
	resultTH = stdout;
	console.log(resultTH);
	});

    response.writeHead(200, {"Contet-Type": "text/html"});
    response.write("<h1>----------</h1>");
    response.write("<p>"+ resultTH +"</p>");
    response.write("<h2>----------</h2p>");
    response.write("<script> ");
    response.write(" window.setInterval('refresh()', 5000);");
    response.write("function refresh() {window.location.reload();}");
//    response.write("");
    response.end("</script>");
});
var port = 80;
server.listen(port);

console.log("Server running at http:localhost:%d", port);
