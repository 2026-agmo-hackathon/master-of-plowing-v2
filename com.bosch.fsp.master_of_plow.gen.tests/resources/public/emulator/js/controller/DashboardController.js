mockCloud.controller('DashboardController', function($state, service ,$timeout, $interval,uploadedFiles, downloadedFiles){
    console.log('DashboardController');

    var vm=this;
    vm.context = {};
    vm.context.uploadfiles=uploadedFiles.data;
    vm.context.downloadedFiles=downloadedFiles.data;

    function fetch_det(){
        $timeout(function(){
        	service.getUploadedfiles().then(function(response) {
                vm.context.uploadfiles=response.data;
            }, function(response) {
                console.log('error');
            }); 
        	service.getDownloadedfiles().then(function(response) {
                vm.context.downloadedFiles=response.data;
            }, function(response) {
                console.log('error');
            }); 
        },3000);
        console.log('updated');
    }
    
    vm.showfile = function showfile(filepath){
    	service.showfile(filepath); 
//        service.showfile(filepath).then(function(response) {
//            console.log('success')
//        }, function(response) {
//            console.log('error');
//        }); 
    } 
    
    $interval(fetch_det,10000);
  
});

    
