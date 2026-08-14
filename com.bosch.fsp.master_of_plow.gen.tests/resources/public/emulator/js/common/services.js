mockCloud.service('service', function(http, $cookies,$window) {

    var serviceManager = {};

    serviceManager.getUploadedfiles = function() {
         return http.get({
             url: '/uploadFiles',
         });

    }

    serviceManager.getDownloadedfiles = function() {
         return http.get({
             url: '/downloadedFiles',
         });

    }

    serviceManager.pushmsgtofeature = function(obj,platform) {
         return http.post({
             url: '/pushmsgtofeature/'+platform,
             data: obj
         });
        console.log('pushed msg to feature');
    }

    serviceManager.showfile =function(path){
    	$window.open( '/showfile/'+path);
//        return http.get({
//          
//           url: 'showfile/'+path,
//           
//       });
   } 
    return serviceManager;
});