//
//  ActivityController.m
//  web
//
//  Created by Jan Hölscher on 12.11.11.
//  Copyright (c) 2011 280 North, Inc. All rights reserved.
//
@import "../../View/ProgressTableDataView.j"

@implementation ActivityController: CPObject
{
  @outlet CPTableView  jobTableView;
  id jsonData;
  
  
}

-(void)awakeFromCib{
  CPLog.debug("awaked from cib!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
  var progressDataView=[ProgressTableDataView new];
  CPLog.debug("ProgressTableDataView:"+progressDataView);
  CPLog.debug("JobTable:"+jobTableView);
  var column = [jobTableView tableColumnWithIdentifier:@"5"];
  CPLog.debug("Column:"+column);
  [column setDataView:progressDataView];

  [self refresh];
  [CPTimer scheduledTimerWithTimeInterval:10 target:self selector:@selector(refresh) userInfo:nil repeats:true];
  
}

- (int)numberOfRowsInTableView:(CPTableView)tabView
{
  var result=0;
  if(jsonData){
    result=jsonData.data.length;
  }
  return result;
}

- (id)tableView:(CPTableView)tabView objectValueForTableColumn:(CPTableColumn)tableColumn row:(int)row
{
  if ([tableColumn identifier] == "icons"){
    // return a CPImage instance that will be displayed in the custom column (CPImageView)
    if (row % 2){
      return _iconImage1;
    }
    else if (row % 3){
      return _iconImage3;
    }
    else{
      return _iconImage2;
    }
  }else{
    // return a CPString instance that will be displayed in a regular column (CPTextField)
    //CPLog.debug([tableColumn identifier]);
    if([tableColumn identifier]==1){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].id ];
    }else if([tableColumn identifier]==2){
      if(jsonData.data[row].created>1){
        return [CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime ];
      }else{
        return ""
      }
      //return [CPString stringWithFormat:@"%s", jsonData.data[row].begintime ];
    }else if([tableColumn identifier]==3){
      if(jsonData.data[row].endtime>1){
        return [CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime ];
      }else{
        return "";
      }
      //return [CPString stringWithFormat:@"%s", jsonData.data[row].endtime ];
      //return [[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime]];
    }else if([tableColumn identifier]==4){
      if(jsonData.data[row].endtime.length>1){
        return [self formatDuration:[[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime]]];
      }else{
        return "";
      }
    }else if([tableColumn identifier]==5){
      if(jsonData.data[row].status=="Processing"){
        return [CPString stringWithFormat:@"%s", jsonData.data[row].progress ];
      }else
        return jsonData.data[row].status;
    }else if([tableColumn identifier]==6){
      if(jsonData.data[row].fps)
        return [CPString stringWithFormat:@"%s", jsonData.data[row].fps ];
      else return "";
    }else if([tableColumn identifier]==7){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].status ];
    }else if([tableColumn identifier]==8){
      return jsonData.data[row];
    }
  }
}
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  //this method is called when the network request returns. the data is the returned
  //information from flickr. we set the array of photo urls as the data to our collection view
  //CPLog.debug("status==200 : "+[_response statusCode]==200);
  
  //if([_response statusCode]==200){
    jsonData=[data objectFromJSON];
    [jobTableView reloadData];
  //}else{
    //alert(data);
  //}
}
- (void)refresh{
  var request = [CPURLRequest requestWithURL:"/api/v1/encoding?full"];
  [request setHTTPMethod:"GET"];
  //CPLog.debug(request.HTTPMethod);
  // see important note about CPJSONPConnection above
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
}

- (void)tableViewSelectionDidChange:(CPNotification)aNotification{
  if(jsonData.data[[[aNotification object] selectedRow]]){
    CPLog.debug("hello new:"+jsonData.data[[[aNotification object] selectedRow]].id);
    selectedid=jsonData.data[[[aNotification object] selectedRow]].id;
    [[CPNotificationCenter defaultCenter]
     postNotificationName:EncodingClicked
     object:self
     userInfo:selectedid];
  }
}


@end
