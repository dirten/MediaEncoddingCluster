/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.CompositeEditor;
import com.google.gwt.editor.client.EditorDelegate;
import com.google.gwt.editor.client.LeafValueEditor;
import com.google.gwt.editor.client.adapters.ListEditor;
import com.google.gwt.user.client.ui.Composite;
import com.sencha.gxt.widget.core.client.container.FlowLayoutContainer;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class VideoPropertyListEditor extends Composite implements
        CompositeEditor<List<PropertyItem>, PropertyItem, PropertyItemEditor>//,
        /*LeafValueEditor<Map<String,String>>*/{

    
    private List<PropertyItem> _currentData;
    private EditorDelegate<List<PropertyItem>> _delegate;
    EditorChain<PropertyItem, PropertyItemEditor> _chain;
    private static final Logger logger = Logger.getLogger(VideoEditor.class.toString());
    private List<PropertyItemEditor> editors=new ArrayList<PropertyItemEditor>();
    private FlowLayoutContainer vlc;

    public VideoPropertyListEditor() {
        vlc = new FlowLayoutContainer();
        initWidget(vlc);
       
    }

    public PropertyItemEditor createEditorForTraversal() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public String getPathElement(PropertyItemEditor subEditor) {
        logger.log(Level.INFO,"subEditor:"+subEditor);
        return "";//"[" + editors.indexOf(subEditor) + "]";
    }

    public void setEditorChain(EditorChain<PropertyItem, PropertyItemEditor> chain) {
        _chain=chain;
    }

    public void flush() {
        for(PropertyItemEditor editor:editors){
            PropertyItem entry=_chain.getValue(editor);
            logger.log(Level.INFO,"flush key="+entry.getKey()+" val:"+entry.getVal());
            //_currentData.put(entry.getKey(), entry.getValue());
        }
    }

    public void onPropertyChange(String... paths) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void setValue(List<PropertyItem> value) {
        //logger.log(Level.INFO,"data:"+value.size());
        _currentData=value;
        for(PropertyItem entry:value){
            PropertyItemEditor editor=new PropertyItemEditor();
            //PropertyItem item=new PropertyItem(entry.getKey(), entry.getValue());
            editors.add(editor);
            _chain.attach(entry, editor);
            vlc.add(editor);
           
        }
    }

    public void setDelegate(EditorDelegate<List<PropertyItem>> delegate) {
        _delegate=delegate;
    }

    public List<PropertyItem> getValue() {
        return _currentData;
    }

    


}
