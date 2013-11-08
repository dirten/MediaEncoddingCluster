/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.editor.client.CompositeEditor;
import com.google.gwt.editor.client.EditorDelegate;
import com.google.gwt.editor.client.IsEditor;
import com.google.gwt.editor.client.LeafValueEditor;
import com.google.gwt.editor.client.adapters.EditorSource;
import com.google.gwt.editor.client.adapters.ListEditor;
import com.google.gwt.user.client.ui.Composite;
import com.google.web.bindery.requestfactory.gwt.client.HasRequestContext;
import com.google.web.bindery.requestfactory.shared.RequestContext;
import com.sencha.gxt.widget.core.client.container.FlowLayoutContainer;
import de.mec.client.model.Video;
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
        CompositeEditor<Map<String,String>, Entry<String,String>, PropertyItemEditor>,
        LeafValueEditor<Map<String,String>>{

    
    private Map<String,String> _currentData;
    private EditorDelegate<Map<String,String>> _delegate;
    EditorChain<Entry<String,String>, PropertyItemEditor> _chain;
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
        //logger.log(Level.INFO,"subEditor:"+subEditor);
        return "";
    }

    public void setEditorChain(EditorChain<Entry<String, String>, PropertyItemEditor> chain) {
        _chain=chain;
    }

    public void flush() {
        for(PropertyItemEditor editor:editors){
            Entry<String,String>entry=_chain.getValue(editor);
            logger.log(Level.INFO,"flush key="+entry.getKey()+" val:"+entry.getValue());
            _currentData.put(entry.getKey(), entry.getValue());
        }
    }

    public void onPropertyChange(String... paths) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void setValue(Map<String, String> value) {
        logger.log(Level.INFO,"data:"+value.size());
        _currentData=value;
        for(Entry<String,String>entry:value.entrySet()){
            PropertyItemEditor editor=new PropertyItemEditor();
            PropertyItem item=new PropertyItem(entry.getKey(), entry.getValue());
            editors.add(editor);
            vlc.add(editor);
            _chain.attach(item, editor);
        }
    }

    public void setDelegate(EditorDelegate<Map<String, String>> delegate) {
        _delegate=delegate;
    }

    public Map<String, String> getValue() {
        return _currentData;
    }

    


}
