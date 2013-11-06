/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.IsEditor;
import com.google.gwt.editor.client.adapters.EditorSource;
import com.google.gwt.editor.client.adapters.ListEditor;
import com.google.gwt.user.client.ui.Composite;
import com.google.web.bindery.requestfactory.gwt.client.HasRequestContext;
import com.google.web.bindery.requestfactory.shared.RequestContext;
import com.sencha.gxt.widget.core.client.container.FlowLayoutContainer;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

/**
 *
 * @author jan.hoelscher
 */
public class VideoPropertyListEditor extends Composite implements IsEditor<ListEditor<Map.Entry<String,String>, PropertyItemEditor>>, HasRequestContext<List<Map.Entry<String,String>>>{
    
    private FlowLayoutContainer vlc;
    
    public VideoPropertyListEditor(){
        vlc=new FlowLayoutContainer();
        initWidget(vlc);
    }
    private class TodoItemEditorSource extends EditorSource<PropertyItemEditor> {
    @Override
    public PropertyItemEditor create(final int index) {
      PropertyItemEditor subEditor = new PropertyItemEditor();
      vlc.insert(subEditor, index);
      /*
      subEditor.addDeleteHandler(new DeleteEventHandler() {
        public void onDeleteEvent(DeleteEvent event) {
          remove(index);
        }
      });*/
      return subEditor;
    }     

    @Override
    public void dispose(PropertyItemEditor subEditor) {
      subEditor.removeFromParent();
      
    }

    @Override
    public void setIndex(PropertyItemEditor editor, int index) {
      vlc.insert(editor, index);
    }
    
  }   
  private ListEditor<Map.Entry<String,String>, PropertyItemEditor> editor = ListEditor.of(new TodoItemEditorSource());
    public ListEditor<Entry<String, String>, PropertyItemEditor> asEditor() {
        return editor;
    }

    public void setRequestContext(RequestContext ctx) {
        throw new UnsupportedOperationException("Not supported yet.");
    }
    
}
