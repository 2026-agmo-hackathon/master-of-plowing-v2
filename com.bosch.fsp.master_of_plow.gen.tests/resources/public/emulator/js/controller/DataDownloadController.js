mockCloud.controller('DataDownloadController', function($state, service, Upload, $timeout) {

    console.log('DataDownloadController');

    var vm = this;
    vm.context = {}
    vm.context.platformoptions= [
        'Cloud','Device'
    ]

    vm.pushmsgtofeature = function(msg,platform) {
        console.log('pushing msg to feature')
        service.pushmsgtofeature(msg,platform).then(function(response) {
            vm.context.msguploadstatus = 'Message sent successfully.';
            vm.msgdata="";
            $timeout(function() {
                vm.context.msguploadstatus = '';
            }, 4000);
            
            }, function(response) {
                console.log('error');
            }); 
    };


    function uploadstatus(uploadmsg) {
        vm.context.uploadstatus = uploadmsg;
        $timeout(function() {
            vm.context.uploadstatus = '';
        }, 4000);
    }

    vm.pushfiletofeature = function(file, platform) {
        if (file && file.size > 0 && platform) {
            vm.context.uploadedfile = file;
            Upload.upload({
                url: '/uploadfile/' + file.name +'/' +platform, //change here for uploadilfe url
                file: file
            }).progress(function(evt) {
                console.log('progress: ' + vm.context.uploadedfile.progress + '% ' + evt.config.file.name);
            }).success(function(data, status, headers, config) {
                if (data) {
                    uploadstatus('    ' + config.file.name + ' uploaded successfully.');
                    console.log('file ' + config.file.name + 'uploaded success. Response: ' + data);
                } else {
                    uploadstatus('   Upload not success. Kindly try again');
                }
            }).catch(function errorCallback() {
                console.log('some error during upload');
                uploadstatus('   Some error during upload');
            });
        }
    }

    
    //pure js not sued line 46 to line 73

    vm.pushfiletofeature1 = function(filedata) {
        console.log('pushing file to feature')
        var f = document.getElementById('file-data').files[0];
        formData = new FormData();
        formData.append("file", f);
        var ajax = new XMLHttpRequest;
        ajax.upload.addEventListener("progress", myProgressHandler, false);
        ajax.addEventListener('load', myOnLoadHandler, false);
        ajax.open('POST', 'uploadfile/' + filename, true);
        ajax.send(formData);
    };

    function myProgressHandler(event) {
        //your code to track upload progress
        // var p = Math.floor(event.loaded/event.total*100);
        // document.title = p+'%';
        console.log('pushing file to feature')
        vm.filupload.status = ' uploading in progress';
    }

    function myOnLoadHandler(event) {
        // your code on finished upload
        console.log('pushing file to feature')
        alert(event.target.responseText);
        vm.filupload.status = 'Completed';
    }
});