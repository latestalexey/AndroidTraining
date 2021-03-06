package com.elegion.android.rx;

import android.app.LoaderManager;
import android.content.Context;
import android.content.Loader;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import rx.Observable;
import rx.Observer;

/**
 * @author Daniel Serdyukov
 */
class RxLcImpl<T> implements LoaderManager.LoaderCallbacks<RxResult<T>> {

    private final Context mContext;

    private final Observable<T> mObservable;

    private final Observer<T> mObserver;

    private final Uri mContentUri;

    public RxLcImpl(@NonNull Context context, @NonNull Observable<T> observable, @NonNull Observer<T> observer,
                    @Nullable Uri contentUri) {
        mContext = context;
        mObservable = observable;
        mObserver = observer;
        mContentUri = contentUri;
    }

    @Override
    public Loader<RxResult<T>> onCreateLoader(int id, Bundle args) {
        return new RxResultLoader<>(mContext, mObservable, mContentUri);
    }

    @Override
    public void onLoadFinished(Loader<RxResult<T>> loader, RxResult<T> data) {
        if (data != null) {
            if (data.getError() != null) {
                mObserver.onError(data.getError());
            } else {
                mObserver.onNext(data.getResult());
            }
        }
        mObserver.onCompleted();
    }

    @Override
    public void onLoaderReset(Loader<RxResult<T>> loader) {
        // Do nothing
    }

}
