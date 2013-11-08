/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import de.mec.client.model.Video;
import de.mec.client.model.Codec;
import de.mec.client.model.Audio;
import de.mec.client.model.Profile;
import de.mec.client.model.Format;
import com.google.gwt.core.client.GWT;
import com.google.gwt.http.client.*;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.web.bindery.autobean.shared.AutoBean;
import com.google.web.bindery.autobean.shared.AutoBeanCodex;
import com.google.web.bindery.autobean.shared.AutoBeanFactory;
import com.google.web.bindery.autobean.shared.AutoBeanFactory.Category;
import com.google.web.bindery.autobean.shared.AutoBeanUtils;
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
public class MHiveService {

    private static final Logger logger = Logger.getLogger(MHiveService.class.toString());
    private static List<Codec> _audioCodecs;
    private static List<Codec> _videoCodecs;
    private static List<Format> _formats;

    interface ProfileList {

        List<Profile> getData();
    }

    public interface ProfileListAutoBeanFactory extends AutoBeanFactory {

        AutoBean<ProfileList> data();
    }

    void getProfiles(final AsyncCallback<java.util.List<Profile>> callback) {
        RequestBuilder builder = new RequestBuilder(RequestBuilder.GET, "/api/v1/profile/");
        builder.setCallback(new RequestCallback() {
            public void onResponseReceived(Request request, Response response) {
                //logger.log(Level.INFO, "response data :" + response.getText());
                ProfileListAutoBeanFactory fac = GWT.create(ProfileListAutoBeanFactory.class);
                AutoBean<ProfileList> bean = AutoBeanCodex.decode(fac, ProfileList.class, response.getText());
                //logger.log(Level.INFO, "response bean :" + bean.as().getData());
                callback.onSuccess(bean.as().getData());
            }

            public void onError(Request request, Throwable exception) {
                callback.onFailure(exception);
                //throw new UnsupportedOperationException("Not supported yet.");
            }
        });
        try {
            builder.send();
            //throw new UnsupportedOperationException("Not supported yet.");
        } catch (RequestException ex) {
            callback.onFailure(ex);
            Logger.getLogger(ProfileList.class.getName()).log(Level.SEVERE, null, ex);
        }

    }
    
    
    //@Category(ProfileCategoryNew.class)
    public interface ProfileResult {
        Profile getData();
    }
    
    public interface ProfileResultAutoBeanFactory extends AutoBeanFactory {
        AutoBean<ProfileResult> data();
    }
    
    
    void getProfile(String profileId, final AsyncCallback<Profile> callback) {
        RequestBuilder builder = new RequestBuilder(RequestBuilder.GET, "/api/v1/profile/" + profileId);
        builder.setCallback(new RequestCallback() {
            public void onResponseReceived(Request request, Response response) {
                logger.log(Level.INFO, "response data :" + response.getText());
                if (response.getStatusCode() == 200) {
                    ProfileResultAutoBeanFactory fac = GWT.create(ProfileResultAutoBeanFactory.class);
                    AutoBean<ProfileResult> bean = AutoBeanCodex.decode(fac, ProfileResult.class, response.getText());
                    logger.log(Level.INFO, "response bean :" + bean.as());
                    callback.onSuccess(bean.as().getData());
                } else {
                    callback.onFailure(new Exception(response.getStatusText()));
                }
            }

            public void onError(Request request, Throwable exception) {
                logger.log(Level.SEVERE, "Error loading profile", exception);
                callback.onFailure(exception);
            }
        });
        try {
            builder.send();
            //throw new UnsupportedOperationException("Not supported yet.");
        } catch (RequestException ex) {
            callback.onFailure(ex);
            logger.log(Level.SEVERE, null, ex);
        }
    }

    void saveProfile(Profile profile, final AsyncCallback<Profile> callback) {
        logger.log(Level.INFO, "profile save :");
        AutoBean<Profile> bean = AutoBeanUtils.getAutoBean(profile);
        String json = AutoBeanCodex.encode(bean).getPayload();
        logger.log(Level.INFO, "json bean :" + json);
        //callback.onSuccess(profile);
        final String url = "/api/v1/profile/" + (profile.getUuid() != null ? profile.getUuid() : "");
        logger.log(Level.INFO, "calling url :" + url);

        RequestBuilder builder = new RequestBuilder(profile.getUuid() != null ? RequestBuilder.PUT : RequestBuilder.POST, url);
        builder.setRequestData(json);
        builder.setCallback(new RequestCallback() {
            public void onResponseReceived(Request request, Response response) {
                logger.log(Level.INFO, "response data :" + response.getText());
                if (response.getStatusCode() == 200) {
                    ProfileResultAutoBeanFactory fac = GWT.create(ProfileResultAutoBeanFactory.class);
                    AutoBean<ProfileResult> bean = AutoBeanCodex.decode(fac, ProfileResult.class, response.getText());
                    logger.log(Level.INFO, "response bean :" + bean.as());
                    callback.onSuccess(bean.as().getData());
                } else {
                    callback.onFailure(new Exception(response.getStatusText()));
                }
            }

            public void onError(Request request, Throwable exception) {
                logger.log(Level.SEVERE, "Error loading profile", exception);
                callback.onFailure(exception);
            }
        });
        try {
            builder.send();
            //throw new UnsupportedOperationException("Not supported yet.");
        } catch (RequestException ex) {
            callback.onFailure(ex);
            logger.log(Level.SEVERE, null, ex);
        }

    }

    interface ProfileFactory extends AutoBeanFactory {

        AutoBean<Profile> profile();

        AutoBean<Audio> audio();

        AutoBean<Video> video();

        AutoBean<Format> format();
    }

    public Profile createProfile() {
        ProfileFactory factory = GWT.create(ProfileFactory.class);
        Profile profile = factory.profile().as();
        profile.setAudio(factory.audio().as());
        profile.setVideo(factory.video().as());
        profile.setFormat(factory.format().as());
        //logger.log(Level.INFO,"Audio from profile :"+profile.getAudio());
        return profile;
    }

    /**
     * ************************************************************************
     *
     * here is the part for the codec loading
     *
     ************************************************************************
     */
    interface CodecList {

        List<Codec> getData();
    }

    interface CodecListAutoBeanFactory extends AutoBeanFactory {

        AutoBean<CodecList> data();
    }

    void getVideoCodecs(final AsyncCallback<java.util.List<Codec>> callback) {
        if (_videoCodecs != null) {
            logger.log(Level.INFO, "returning video codec data from cache");

            callback.onSuccess(_videoCodecs);
            return;
        }
        RequestBuilder builder = new RequestBuilder(RequestBuilder.GET, "/api/v1/codec/");
        builder.setCallback(new RequestCallback() {
            public void onResponseReceived(Request request, Response response) {
                CodecListAutoBeanFactory fac = GWT.create(CodecListAutoBeanFactory.class);
                AutoBean<CodecList> bean = AutoBeanCodex.decode(fac, CodecList.class, response.getText());
                List<Codec> allCodecs = bean.as().getData();
                List<Codec> videoCodecs = new ArrayList<Codec>();

                for (Codec codec : allCodecs) {
                    if (codec.getCodecType() == 0) {
                        videoCodecs.add(codec);
                    }
                }
                _videoCodecs = videoCodecs;
                callback.onSuccess(videoCodecs);
            }

            public void onError(Request request, Throwable exception) {
                callback.onFailure(exception);
            }
        });
        try {
            builder.send();
        } catch (RequestException ex) {
            callback.onFailure(ex);
            Logger.getLogger(ProfileList.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    void getAudioCodecs(final AsyncCallback<java.util.List<Codec>> callback) {
        if (_audioCodecs != null) {
            logger.log(Level.INFO, "returning audio codec data from cache");
            callback.onSuccess(_audioCodecs);
            return;
        }
        RequestBuilder builder = new RequestBuilder(RequestBuilder.GET, "/api/v1/codec/");
        builder.setCallback(new RequestCallback() {
            public void onResponseReceived(Request request, Response response) {
                CodecListAutoBeanFactory fac = GWT.create(CodecListAutoBeanFactory.class);
                AutoBean<CodecList> bean = AutoBeanCodex.decode(fac, CodecList.class, response.getText());
                List<Codec> allCodecs = bean.as().getData();
                List<Codec> audioCodecs = new ArrayList<Codec>();

                for (Codec codec : allCodecs) {
                    if (codec.getCodecType() == 1) {
                        audioCodecs.add(codec);
                    }
                }
                _audioCodecs = audioCodecs;
                callback.onSuccess(audioCodecs);
            }

            public void onError(Request request, Throwable exception) {
                callback.onFailure(exception);
            }
        });
        try {
            builder.send();
        } catch (RequestException ex) {
            callback.onFailure(ex);
            Logger.getLogger(ProfileList.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * ************************************************************************
     *
     * here is the part for the format list loading
     *
     ************************************************************************
     */
    interface FormatList {

        List<Format> getData();
    }

    interface FormatListAutoBeanFactory extends AutoBeanFactory {

        AutoBean<FormatList> data();
    }

    void getFormats(final AsyncCallback<java.util.List<Format>> callback) {
        if (_formats != null) {
            logger.log(Level.INFO, "returning format data from cache");

            callback.onSuccess(_formats);
            return;
        }
        RequestBuilder builder = new RequestBuilder(RequestBuilder.GET, "/api/v1/format/");
        builder.setCallback(new RequestCallback() {
            public void onResponseReceived(Request request, Response response) {
                logger.log(Level.INFO, "response data :" + response.getText());
                FormatListAutoBeanFactory fac = GWT.create(FormatListAutoBeanFactory.class);
                AutoBean<FormatList> bean = AutoBeanCodex.decode(fac, FormatList.class, response.getText());
                List<Format> all = bean.as().getData();
                logger.log(Level.INFO, "response bean :" + bean.as());
                _formats = all;
                callback.onSuccess(all);
            }

            public void onError(Request request, Throwable exception) {
                callback.onFailure(exception);
            }
        });
        try {
            builder.send();
        } catch (RequestException ex) {
            callback.onFailure(ex);
            Logger.getLogger(ProfileList.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

}
