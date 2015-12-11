package ru.samples.itis.githubclient.di;

import javax.inject.Singleton;

import dagger.Component;
import ru.samples.itis.githubclient.MainActivity;
import ru.samples.itis.githubclient.di.module.ServerModule;

/**
 * @author Artur Vasilov
 */
@Singleton
@Component(modules = ServerModule.class)
public interface DaggerGraph {

    void injectMainActivity(MainActivity mainActivity);

}