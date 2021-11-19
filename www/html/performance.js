function disableUI()
{
   document.getElementById("startMeasureButton").disabled = true;
   document.getElementById("progressBar").disabled = true;
   document.getElementById("numCycles").disabled = true;
}

function enableUI()
{
   document.getElementById("startMeasureButton").disabled = false;
   document.getElementById("progressBar").disabled = false;
   document.getElementById("numCycles").disabled = false;
}

function output(message) 
{
    var output = document.getElementById("output");
    if(output != null)
    {
        output.innerHTML = output.innerHTML + message + "\n";
    }
}

function handleWebChannel(socket)
{
    new QWebChannel(socket, function(channel) 
    {
        enableUI();
        window.simpleMirror = channel.objects.SimpleMirror;

        output("Number of measure cycles: " + simpleMirror.numDesiredCycles);
        document.getElementById("progressBar").max = simpleMirror.numDesiredCycles;
        document.getElementById("numCycles").value = simpleMirror.numDesiredCycles;

        document.getElementById("numCycles").addEventListener('change', function(e){
           simpleMirror.numDesiredCycles = e.target.value;
           document.getElementById("progressBar").max = e.target.value;
        });

        document.getElementById("startMeasureButton").onclick = function()
        {
            output("startMeasureButton clicked");
            simpleMirror.startMeasuring();
            this.disabled = true;
        }

        simpleMirror.toFrontend.connect(function(value){
           simpleMirror.fromFrontend(value);
           document.getElementById("progressBar").value = value;
        });

        simpleMirror.measuredMicrosecondsChanged.connect(function(){
           document.getElementById("startMeasureButton").disabled = false;
           output("Measured " + simpleMirror.measuredMicroseconds + " us");
        });
    });
}

function connectToServer() 
{
    if (location.search != "")
        var baseUrl = (/[?&]webChannelBaseUrl=([A-Za-z0-9\-:/\.]+)/.exec(location.search)[1]);
    else
        var baseUrl = "ws://localhost:12345";

    output("Connecting to WebSocket server at " + baseUrl + ".");
    var socket = new WebSocket(baseUrl);

    socket.onclose = function() 
    {
        output("web channel closed");
        output('Socket is closed. Reconnect will be attempted in 1 second.');
        disableUI();
        setTimeout(function() {
            output("Timeout -- reconnecting")
            connectToServer();
        }, 100);
    };

    socket.onerror = function(error) 
    {
        output("web channel error: " + error);
        socket.close();
    };

    socket.onopen = function(){
        output("WebSocket connected, setting up QWebChannel.");
        handleWebChannel(socket);
    }
}


window.onload = function()
{
    var textarea = document.getElementById("output");
    textarea.scrollTop = textarea.scrollHeight;
    if(typeof qt !== 'undefined')
    {
        handleWebChannel(qt.webChannelTransport)
    }
    else
    {
        disableUI();
        connectToServer();
    }
}